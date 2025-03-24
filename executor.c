#include "cccl.h"

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


static struct cccl_Variables globals = {0};
static struct cccl_Function functions[52] = {0};
static struct cccl_Stack stack = {0};

static void expand_stack(void)
{
    if (!stack.buffer)
    {
        stack.allocated = sizeof(*stack.buffer) * stack.length;
        stack.buffer = malloc(stack.allocated);
    } else if (stack.length > stack.allocated / sizeof(*stack.buffer))
    {
        stack.allocated = sizeof(*stack.buffer) * stack.length;
        stack.buffer = realloc(stack.buffer, stack.allocated);
    }
    assert(stack.buffer && "buy more ram");
}

static size_t geti(char name)
{
    return islower(name) ? name - 'a' : name - 'A' + 26;
}

static short *get_variable(char name, struct cccl_Variables *scope)
{
    size_t i = geti(name);
    if (scope->used[i])
        return &scope->buffer[i];
    if (globals.used[i])
        return &globals.buffer[i];
    return NULL;
}

enum cccl_ExecutorStatus cccl_execute(struct cccl_Node *code, struct cccl_Variables *scope)
{
    switch (code->type)
    {
    case cccl_Node_CODE:
    {
        int res;
        for (size_t i = 0; i < code->in_length; ++i)
            if ((res = cccl_execute(code->in[i], scope)) != 0)
                goto code_end;
code_end:
        if (res == cccl_Executor_ERROR)
            return res;
    } break;
    case cccl_Node_PUSHZERO:
    {
        ++stack.length;
        expand_stack();
        stack.buffer[stack.length - 1] = 0;
    } break;
    case cccl_Node_INCREMENT:
    {
        assert(stack.length >= 1);
        ++stack.buffer[stack.length - 1];
    } break;
    case cccl_Node_DECREMENT:
    {
        assert(stack.length >= 1);
        --stack.buffer[stack.length - 1];
    } break;
    case cccl_Node_ADD:
    {
        --stack.length;
        stack.buffer[stack.length - 1] += stack.buffer[stack.length];
    } break;
    case cccl_Node_SUBTRACT:
    {
        --stack.length;
        stack.buffer[stack.length - 1] -= stack.buffer[stack.length];
    } break;
    case cccl_Node_REVERSE:
    {
        size_t torev;
        if (code->value == '_')
        {
            torev = stack.length;
        } else
        {
            short *p = get_variable(code->value, scope);
            if (!p)
                errx(1, "Variable %c doesn't exist!", code->value);
            if (*p > stack.length || *p < 0)
                errx(1, "Cannot reverse %d elements in a stack of length %lu", *p, stack.length);
            torev = *p;
        }

        short *buffer = stack.buffer + (stack.length - torev);
        for (size_t i = 0; i < torev / 2; ++i)
        {
            buffer[i] ^= buffer[torev - i - 1];
            buffer[torev - i - 1] ^= buffer[i];
            buffer[i] ^= buffer[torev - i - 1];
        }
    } break;
    case cccl_Node_ASSIGN:
    {
        assert(stack.length >= 1);
        --stack.length;
        if (code->value == '_')
            break;

        size_t i = geti(code->value);
        short *p = get_variable(code->value, scope);
        if (!p)
        {
            globals.used[i] = 1;
            p = &globals.buffer[i];
        }
        *p = stack.buffer[stack.length];
    } break;
    case cccl_Node_DELETE:
    {
        if (code->value == '_')
            errx(1, "_ is not allowed with '%c'", '!');

        size_t i = geti(code->value);
        if (scope->used[i])
            scope->used[i] = 0;
        else if (!globals.used[i])
            errx(1, "Cannot delete non-existent variable %c", code->value);
        else
            globals.used[i] = 0;
    } break;
    case cccl_Node_PUSHVAR:
    {
        ++stack.length;
        expand_stack();

        if (code->value == '_')
            errx(1, "_ is not allowed with '%c'", '$');

        short *p = get_variable(code->value, scope);
        if (!p)
            errx(1, "Cannot push %c on the stack is it doesn't exist", code->value);
        stack.buffer[stack.length - 1] = *p;
    } break;
    case cccl_Node_ASSIGNLOCAL:
    {
        if (code->value == '_')
            errx(1, "_ is not allowed with '%c'", '&');

        size_t i = geti(code->value);
        scope->used[i] = 1;
        scope->buffer[i] = 0;
    } break;
    case cccl_Node_OUTPUT:
    {
        if (code->value == '_')
            errx(1, "_ is not allowed with '%c'", '<');

        short *p = get_variable(code->value, scope);
        if (!p)
            errx(1, "Cannot print non-existent variable %c", code->value);

        putchar(*p);
    } break;
    case cccl_Node_INPUT:
    {
        if (code->value == '_')
            errx(1, "_ is not allowed with '%c'", '>');

        short *p = get_variable(code->value, scope);
        if (!p)
            errx(1, "Cannot save input in a non-existent variable %c", code->value);

        int c = getchar();
        if (c < 0)
            err(1, "getchar()");

        *p = c;
    } break;
    case cccl_Node_PROCEDURE:
    {
        functions[geti(code->value)] = (struct cccl_Function)
        {
            .body = code->in,
            .length = code->in_length,
        };
    } break;
    case cccl_Node_CALL:
    {
        if (functions[geti(code->value)].body == NULL)
            errx(1, "Cannot call non-existent function %c", code->value);

        size_t i = geti(code->value);
        struct cccl_Variables localscope = {0};
        int res;
        for (size_t j = 0; j < functions[i].length; ++j)
            if ((res = cccl_execute(functions[i].body[j], &localscope)) != 0)
                goto call_end;
call_end:
        if (res == cccl_Executor_ERROR)
            return res;
    } break;
    case cccl_Node_INFINITE:
    {
        int res;
        if (code->value == '_')
            for (;;)
                for (size_t i = 0; i < code->in_length; ++i)
                    if ((res = cccl_execute(code->in[i], scope)) != 0)
                        goto infinite_end;

        short *p = get_variable(code->value, scope);
        if (!p)
            errx(1, "Cannot loop using non-existent variable %c", code->value);

        while (*p > 0)
            for (size_t i = 0; i < code->in_length; ++i)
                if ((res = cccl_execute(code->in[i], scope)) != 0)
                    goto infinite_end;
infinite_end:
        if (res == cccl_Executor_ERROR)
            return res;
    } break;
    case cccl_Node_REPEAT:
    {
        short *p = get_variable(code->value, scope);
        if (!p)
            errx(1, "Cannot loop using non-existent variable %c", code->value);
        else if (*p < 0)
            errx(1, "Cannot iterate %c=%d times", code->value, *p);

        int res;
        for (size_t i = 0; i < *p; ++i)
            for (size_t j = 0; j < code->in_length; ++j)
                if ((res = cccl_execute(code->in[j], scope)) != 0)
                    goto repeat_end;
repeat_end:
        if (res == cccl_Executor_ERROR)
            return res;
    };
    case cccl_Node_CONDITIONAL:
    {
    } break;
    }

    return 0;
}
