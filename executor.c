#include "cccl.h"

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static struct cccl_Variables globals = {0};
static struct cccl_Function functions[52] = {0};
static struct cccl_Stack stack = {0};

static struct
{
    char *buf;
    size_t length;
} line = {0};

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

static char getnamebyi(size_t i)
{
    return i < 26 ? i + 'a' : i - 26 + 'A';
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

void cccl_dump(FILE *f)
{
    fputs("Globals:\n", f);
    for (size_t i = 0; i < 52; ++i)
        if (globals.used[i])
            fprintf(f, "  %c=%d\n", getnamebyi(i), globals.buffer[i]);
    fputs("Functions:\n", f);
    for (size_t i = 0; i < 52; ++i)
        if (functions[i].body)
            fprintf(f, "  %c, %lu nodes\n", getnamebyi(i), functions[i].length);
    fputs("Stack:\n", f);
    for (size_t i = 0; i < stack.length; ++i)
        fprintf(f, "  %d\n", stack.buffer[i]);
}

enum cccl_ExecutorStatus cccl_execute(struct cccl_Node *code, struct cccl_Variables *scope, size_t depth)
{
    if (verbose)
        if (code->value)
            fprintf(stderr, "Executing %s with %d [%c], %lu nodes, depth %lu\n", cccl_strnode(code->type), code->value, code->value, code->in_length, depth);
        else
            fprintf(stderr, "Executing %s, %lu nodes, depth %lu\n", cccl_strnode(code->type), code->in_length, depth);
    if (interactive)
    {
        ssize_t length = getline(&line.buf, &line.length, stdin);
        if (length == -1 && ferror(stdin))
            err(1, "getline()");
        if (!strcmp(line.buf, "d\n"))
        {
            fputs("Locals:\n", stderr);
            for (size_t i = 0; i < 52; ++i)
                if (scope->used[i])
                    fprintf(stderr, "  %c=%d\n", getnamebyi(i), scope->buffer[i]);
            cccl_dump(stderr);
        }
    }
    switch (code->type)
    {
    case cccl_Node_CODE:
    {
        enum cccl_ExecutorStatus res;
        for (size_t i = 0; i < code->in_length; ++i)
            switch ((res = cccl_execute(code->in[i], scope, depth + 1)))
            {
            case cccl_Executor_ERROR: return res;
            case cccl_Executor_CONTINUE: return cccl_Executor_ERROR;
            case cccl_Executor_END: goto end;
            }
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
        assert(stack.length >= 2);
        --stack.length;
        stack.buffer[stack.length - 1] += stack.buffer[stack.length];
    } break;
    case cccl_Node_SUBTRACT:
    {
        assert(stack.length >= 2);
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
        if (c < 0 && ferror(stdin))
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
        enum cccl_ExecutorStatus res;
        for (size_t j = 0; j < functions[i].length; ++j)
            switch ((res = cccl_execute(functions[i].body[j], &localscope, depth + 1)))
            {
            case cccl_Executor_ERROR: return res;
            case cccl_Executor_CONTINUE: return cccl_Executor_ERROR;
            case cccl_Executor_END: goto end;
            }
    } break;
    case cccl_Node_INFINITE:
    {
        short n = 1, *p;
        if (code->value == '_')
        {
            p = &n;
        } else
        {
            p = get_variable(code->value, scope);
            if (!p)
                errx(1, "Cannot loop over non-existent variable %c", code->value);
        }

        enum cccl_ExecutorStatus res;
        while (*p > 0)
            for (size_t i = 0; i < code->in_length; ++i)
                switch ((res = cccl_execute(code->in[i], scope, depth + 1)))
                {
                case cccl_Executor_ERROR: return res;
                case cccl_Executor_CONTINUE: break;
                case cccl_Executor_END: goto end;
                }
    } break;
    case cccl_Node_REPEAT:
    {
        short *p = get_variable(code->value, scope);
        if (!p)
            errx(1, "Cannot loop over non-existent variable %c", code->value);
        else if (*p < 0)
            errx(1, "Cannot iterate %c=%d times", code->value, *p);

        short n = *p;
        enum cccl_ExecutorStatus res;
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < code->in_length; ++j)
                switch ((res = cccl_execute(code->in[j], scope, depth + 1)))
                {
                case cccl_Executor_ERROR: return res;
                case cccl_Executor_CONTINUE: break;
                case cccl_Executor_END: goto end;
                }
    } break;
    case cccl_Node_CONDITIONAL:
    {
        assert(stack.length >= 1);
        short *p = get_variable(code->value, scope);
        if (code->value == '_')
            errx(1, "_ is not allowed with '%c'", '?');
        if (!p)
            errx(1, "Cannot check non-existent variable %c", code->value);

        enum cccl_ExecutorStatus res;
        if (stack.buffer[stack.length - 1] == *p)
            for (size_t i = 0; i < code->in_length; ++i)
                if ((res = cccl_execute(code->in[i], scope, depth + 1)) != 0)
                    return res;
    } break;
    case cccl_Node_END:
    {
        return cccl_Executor_END;
    } break;
    case cccl_Node_CONTINUE:
    {
        return cccl_Executor_CONTINUE;
    } break;
    }

end:
    return 0;
}
