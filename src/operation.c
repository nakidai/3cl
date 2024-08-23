#include "operation.h"
#include "types.h"

#include <cvector/cvector.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cccl.h"
#include "utils.h"


static cccl *pc;
static bool is_comment;
static i32  current;

static const s8 *const name_allowed             = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const s8 *const all_allowed              = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_{}[]()?;@=!&$%<>^+-*~#:/ \n\t";
static const s8 *const skippable                = " \t\n";
static const s8 *const open_brackets            = "[({?";
static const s8 *const close_brackets           = "])};";

static void check_symbol(void)
{
    if (!strchr(all_allowed, pc->code[pc->ep]))
        die(1, "illegal character (%u at %d)", pc->code[pc->ep], pc->ep);
}

static s8 read_operand(bool allow_underscore)
{
    i32 start = pc->ep;
    for (++pc->ep; pc->ep < pc->size; ++pc->ep)
    {
        check_symbol();
        if (strchr(skippable, pc->code[pc->ep]))
            continue;
        if (!strchr(allow_underscore ? name_allowed : name_allowed + 1, pc->code[pc->ep]))
            die(
                1, "invalid operand name (%c at %d for %c at %d)",
                pc->code[pc->ep], pc->ep,
                pc->code[start], start
            );
        return pc->code[pc->ep];
    }
    die(
        1, "no operand (for %c at %d)",
        pc->code[start], start
    );
}

static void require_stack_size(u32 size)
{
    if (cvector_size(pc->stack) < size)
        die(1, "stack size is %d, but %d is required (%c at %d)", cvector_size(pc->stack), size, pc->code[current], current);
}

static void op_noth(void) {}

static void op_comment(void)
{
    is_comment = !is_comment;
}

static void op_pushzero(void)
{
    cvector_push_back(pc->stack, 0);
}

static void op_increment(void)
{
    require_stack_size(1);
    ++*cvector_back(pc->stack);
}

static void op_decrement(void)
{
    require_stack_size(1);
    --*cvector_back(pc->stack);
}

static void op_add(void)
{
    require_stack_size(2);
    *(cvector_back(pc->stack) - 1) += *cvector_back(pc->stack);
    cvector_pop_back(pc->stack);
}

static void op_sub(void)
{
    require_stack_size(2);
    *(cvector_back(pc->stack) - 1) -= *cvector_back(pc->stack);
    cvector_pop_back(pc->stack);
}

static void op_reverse(void)
{
    s8 operand = read_operand(true);
    i16 *to_reverse = NULL;
    i32 size;

    if (operand == '_')
        size = cvector_size(pc->stack);
}

void cccl_operation_init(cccl *pcp)
{
    pc = pcp;
    is_comment = false;
    current = 0;
}

cccl_operation cccl_select_operation(void)
{
    if (is_comment && pc->code[pc->ep] == '\n')
        return op_comment;
    if (is_comment)
        return op_noth;

    current = pc->ep;
    check_symbol();

    switch (pc->code[pc->ep])
    {
    case ' ':  /* FALLTHROUGH */
    case '\t': /* FALLTHROUGH */
    case ';':  return op_noth;
    case '/':  return op_comment;
    case '^':  return op_pushzero;
    case '+':  return op_increment;
    case '-':  return op_decrement;
    default:   die(1, "You shouldn't be there!");
    }
}
