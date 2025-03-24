#include "cccl.h"

#include <assert.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


static enum cccl_NodeType get_nodetype(char c)
{
    switch (c)
    {
#define X(char, name) case char: return cccl_Node_##name
        X('^', PUSHZERO);
        X('+', INCREMENT);
        X('-', DECREMENT);
        X('*', ADD);
        X('~', SUBTRACT);
        X('%', REVERSE);
        X('=', ASSIGN);
        X('!', DELETE);
        X('$', PUSHVAR);
        X('&', ASSIGNLOCAL);
        X('<', OUTPUT);
        X('>', INPUT);
        X('{', PROCEDURE);
        X('}', PROCEDURE);
        X('@', CALL);
        X('(', INFINITE);
        X(')', INFINITE);
        X('[', REPEAT);
        X(']', REPEAT);
        X('#', END);
        X(':', CONTINUE);
        X('?', CONDITIONAL);
        X(';', CONDITIONAL);
#undef X
    }
    err(1, "Unrecognized command: [%d] %c", c, c);
}


struct cccl_Node *cccl_parse(struct cccl_Token tokens[], size_t tokens_length, enum cccl_NodeType type, char value)
{
    struct cccl_Node *res = malloc(sizeof(*res));
    *res = (struct cccl_Node)
    {
        .type = type,
        .in = NULL,
        .in_length = 0,
        .value = value,
    };

    for (size_t i = 0; i < tokens_length; ++i)
    {
        if (verbose)
            fprintf(stderr, "T:[%c:%d] ", tokens[i].value, tokens[i].type);
        switch (tokens[i].type)
        {
        case cccl_Token_COMMAND: case cccl_Token_COMMANDWITHARG: case cccl_Token_BLOCKSTART:
        {
            if (!res->in)
            {
                res->in_length = 1;
                res->in = malloc(sizeof(*res->in));
            } else
            {
                ++res->in_length;
                res->in = realloc(res->in, sizeof(*res->in) * res->in_length);
            }
        } break;
        }

        switch (tokens[i].type)
        {
        case cccl_Token_COMMAND:
        {
            res->in[res->in_length - 1] = malloc(sizeof(struct cccl_Node));
            *res->in[res->in_length - 1] = (struct cccl_Node)
            {
                .type = get_nodetype(tokens[i].value),
            };
        } break;
        case cccl_Token_COMMANDWITHARG:
        {
            assert(i + 1 != tokens_length);
            assert(tokens[i + 1].type == cccl_Token_IDENTIFIER);
            res->in[res->in_length - 1] = malloc(sizeof(struct cccl_Node));
            *res->in[res->in_length - 1] = (struct cccl_Node)
            {
                .type = get_nodetype(tokens[i].value),
                .value = tokens[i + 1].value,
            };
        } break;
        case cccl_Token_BLOCKSTART:
        {
            assert(i > 0);
            assert(tokens[i - 1].type == cccl_Token_IDENTIFIER);
            char opening = tokens[i].value, closing;
            switch (tokens[i].value)
            {
            break; case '{': closing = '}';
            break; case '(': closing = ')';
            break; case '[': closing = ']';
            break; case '?': closing = ';';
            }

            size_t oldi = i++;
            int depth = 1;
            for (; i < tokens_length; ++i)
            {
                if (verbose)
                    fprintf(stderr, "S:[%c %c %d] ", tokens[i].value, closing, depth);
                if (tokens[i].value == opening)
                    ++depth;
                else if (tokens[i].value == closing)
                    --depth;

                if (depth == 0)
                    goto end;
            }
            errx(1, "No matching bracket for %c", opening);

end:
            putchar('\n');
            puts("Exploring inner...");
            res->in[res->in_length - 1] = cccl_parse(
                tokens + oldi + 1,
                i - oldi - 1,
                get_nodetype(tokens[i].value),
                tokens[oldi - 1].value
            );
        } break;
        }
    }

    return res;
}
