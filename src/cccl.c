#include "cccl.h"
#include "types.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cvector.h"
#include "platform/getch.h"
#include "utils.h"


static cccl pc = {0};
static const s8 *const name_allowed  = "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const s8 *const name_allowed_ = "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
static const s8 *const all_allowed   = "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_{}[]()?;@=!&$%<>^+-*~#:/ \n\t";
static const s8 *const skippable     = " \t\n";

static void cccl_varpair_free(void *p)
{
    cvector_free(*(cccl_varpair **)p);
}

void cccl_init(s8 *filename)
{
    memset(&pc, 0, sizeof(pc));

    cvector_init(pc.stack,      1, NULL);
    cvector_init(pc.variables,  1, NULL);
    cvector_init(pc.procedures, 1, NULL);
    cvector_init(pc.lv_stack,   1, cccl_varpair_free);
    cvector_init(pc.ep_stack,   1, NULL);
    if (errno) die(1, strerror(errno));

    pc.filename = filename;
}

void cccl_free()
{
    cvector_free(pc.stack);
    cvector_free(pc.variables);
    cvector_free(pc.procedures);
    cvector_free(pc.lv_stack);
    cvector_free(pc.ep_stack);
    free(pc.code);
}

void cccl_read(void)
{
    s8 chr;
    FILE *file = fopen(pc.filename, "r");
    if (errno) die(1, strerror(errno));
        fseek(file, 0, SEEK_END);
        pc.size = ftell(file);
        fseek(file, 0, SEEK_SET);
        pc.code = calloc(pc.size + 1, sizeof(s8));

        for (u32 i = 0; i < pc.size; ++i)
        {
            if ((chr = fgetc(file)) == EOF)
            {
                if (errno)
                {
                    fclose(file);
                    die(1, strerror(errno));
                }
                else
                {
                    break;
                }
            }
            pc.code[i] = chr;
        }
    fclose(file);
}

static void check_symbol(void)
{
    if (!strchr(all_allowed, pc.code[pc.ep]))
        die(1, "illegal character (%u at %d)", pc.code[pc.ep], pc.ep);
}

static void require_stack_size(u32 size, i32 start)
{
    if (cvector_size(pc.stack) < size)
        die(1, "stack size is %d, but %d is required (%c at %d)", cvector_size(pc.stack), size, pc.code[start], start);
}

static i32 require_variable_local(s8 name)
{
    if (cvector_size(pc.lv_stack) > 0)
        for (i32 i = 0; i < cvector_size(pc.lv_stack[cvector_size(pc.lv_stack) - 1]); ++i)
            if (pc.lv_stack[cvector_size(pc.lv_stack) - 1][i].name == name)
                return i;
    return -1;
}

static i32 require_variable(s8 name)
{
    for (i32 i = 0; i < cvector_size(pc.variables); ++i)
        if (pc.variables[i].name == name)
            return i;
    return -1;
}

static i32 require_procedure(s8 name)
{
    for (i32 i = 0; i < cvector_size(pc.procedures); ++i)
        if (pc.procedures[i].name == name)
            return i;
    return -1;
}

static s8 read_operand(bool allow_underscore)
{
    i32 start = pc.ep;
    for (++pc.ep; pc.ep < pc.size; ++pc.ep)
    {
        check_symbol();
        if (strchr(skippable, pc.code[pc.ep])) continue;
        if (!strchr(allow_underscore ? name_allowed_ : name_allowed, pc.code[pc.ep]))
            die(1, "invalid operand name (%c at %d for %c at %d)", pc.code[pc.ep], pc.ep, pc.code[start], start);
        return pc.code[pc.ep];
    }
    die(1, "no operand (for %c at %d)", pc.code[start], start);
}

void cccl_run(void)
{
    i32 variable, start, var;
    cccl_varpair varpair;
    cccl_pointer pointer;
    s8 operand;
    bool is_comment;
    for (pc.ep = 0; pc.ep < pc.size; ++pc.ep)
    {
        if (is_comment && pc.code[pc.ep] == '\n')
            is_comment = false;
        else if (is_comment)
            continue;
        check_symbol();
        start = pc.ep;
        switch (pc.code[pc.ep])
        {
        case ' ':
        case '\t':
            break;
        case '/':
            is_comment = true;
            break;
        case '^':
            cvector_push_back(pc.stack, 0);
            break;
        case '+':
            require_stack_size(1, pc.ep);
            ++pc.stack[cvector_size(pc.stack) - 1];
            break;
        case '-':
            require_stack_size(1, pc.ep);
            --pc.stack[cvector_size(pc.stack) - 1];
            break;
        case '*':
            require_stack_size(2, pc.ep);
            pc.stack[cvector_size(pc.stack) - 2] += pc.stack[cvector_size(pc.stack) - 1];
            cvector_pop_back(pc.stack);
            break;
        case '~':
            require_stack_size(2, pc.ep);
            pc.stack[cvector_size(pc.stack) - 2] -= pc.stack[cvector_size(pc.stack) - 1];
            cvector_pop_back(pc.stack);
            break;
        case '%':
            operand = read_operand(true);
            i16 *to_reverse = NULL;
            i32 size;
            if (operand == '_')
            {
                size = cvector_size(pc.stack);
            }
            else
            {
                if ((var = require_variable_local(operand)) == -1)
                {
                    if ((var = require_variable(operand)) == -1)
                        die(1, "no %c variable found (for %% at %d)", operand, start);
                    else
                        size = pc.lv_stack[cvector_size(pc.lv_stack) - 1][var].value;
                } else
                {
                    size = pc.variables[var].value;
                }
            }
            if (size < 1)
                die(1, "invalid argument %d, should be not less than 1 (for %% at %d)", size, start);
            require_stack_size(size, start);
            cvector_init(to_reverse, size, NULL);
            for (int i = 0; i < size; ++i)
            {
                cvector_push_back(to_reverse, pc.stack[cvector_size(pc.stack) - 1]);
                cvector_pop_back(pc.stack);
            }
            for (int i = 0; i < size; ++i)
                cvector_push_back(pc.stack, to_reverse[i]);
            cvector_free(to_reverse);
            break;
        case '=':
            operand = read_operand(true);
            require_stack_size(1, start);
            varpair = (cccl_varpair){.name=operand, .value=pc.stack[cvector_size(pc.stack) - 1]};
            if (operand != '_')
            {
                if ((var = require_variable_local(operand)) == -1)
                    if ((var = require_variable(operand)) == -1)
                        cvector_push_back(pc.variables, varpair);
                    else
                        pc.variables[var] = varpair;
                else
                    pc.lv_stack[cvector_size(pc.lv_stack) - 1][var] = varpair;
            }
            cvector_pop_back(pc.stack);
            break;
        case '!':
            operand = read_operand(false);
            if ((var = require_variable_local(operand)) == -1)
                if ((var = require_variable(operand)) == -1)
                    die(1, "no %c variable found (for ! at %d)", operand, start);
                else
                    cvector_erase(pc.variables, var);
            else
                cvector_erase(pc.lv_stack[cvector_size(pc.lv_stack) - 1], var);
            break;
        case '$':
            operand = read_operand(false);
            i16 value;
            if ((var = require_variable_local(operand)) == -1)
                if ((var = require_variable(operand)) == -1)
                    die(1, "no %c variable found (for ! at %d)", operand, start);
                else
                    value = pc.variables[var].value;
            else
                value = pc.lv_stack[cvector_size(pc.lv_stack) - 1][var].value;
            cvector_push_back(pc.stack, value);
            break;
        case '&':
            operand = read_operand(false);
            if (cvector_size(pc.lv_stack) == 0)
                die(1, "local variables cannot be created in global scope (& at %d)", start);
            varpair = (cccl_varpair){.name=operand, .value=0};
            if (require_variable_local(operand) == -1)
                cvector_push_back(pc.lv_stack[cvector_size(pc.lv_stack) - 1], varpair);
            break;
        case '<':
            operand = read_operand(false);
            if ((var = require_variable_local(operand)) == -1)
                if ((var = require_variable(operand)) == -1)
                    die(1, "no %c variable found (for < at %d)", operand, start);
                else
                    printf("%c", pc.variables[var].value);
            else
                printf("%c", pc.lv_stack[cvector_size(pc.lv_stack) - 1][var].value);
            break;
        case '>':
            operand = read_operand(false);
            i16 *to_store;
            if ((var = require_variable_local(operand)) == -1)
                if ((var = require_variable(operand)) == -1)
                    die(1, "no %c variable found (for > at %d)", operand, start);
                else
                    to_store = &pc.variables[var].value;
            else
                to_store = &pc.lv_stack[cvector_size(pc.lv_stack) - 1][var].value;
            *to_store = getch();
            break;
        case '@':
            operand = read_operand(false);
            if ((var = require_procedure(operand)) == -1)
                die(1, "no %c procedure found (for @ at %d)", operand, start);
            pointer = (cccl_pointer){.value=pc.ep, .is_loop=false};
            cvector_push_back(pc.ep_stack, pointer);
            pc.ep = pc.procedures[var].value;
            break;
        case '#':
            if (cvector_size(pc.lv_stack) == 0)
            {
                exit(0);
            }
            pc.ep = pc.ep_stack[cvector_size(pc.ep_stack) - 1].value;
            if (pc.ep_stack[cvector_size(pc.ep_stack) - 1].is_loop)
                cvector_pop_back(pc.ep_stack);
            cvector_pop_back(pc.ep_stack);
            break;
        }
    }
    printf("Stack: ^^^\n");
    for (int i = 0; i < cvector_size(pc.stack); ++i)
        printf("%d\n", pc.stack[i]);
    printf("Variables:\n");
    for (int i = 0; i < cvector_size(pc.variables); ++i)
        printf("%c:%d\n", pc.variables[i].name, pc.variables[i].value);
}
