#include "cccl.h"

#include <assert.h>
#include <err.h>
#include <stddef.h>


size_t tokenize(const char *code, size_t size, struct cccl_Token tokens[], size_t tokens_length)
{
    size_t i = 0, tokeni = 0;
    int comment = 0;

    do
    {
        switch (code[i])
        {
#define X(name)                                 \
    {                                           \
        if (comment)                            \
            break;                              \
        tokens[tokeni++] = (struct cccl_Token)  \
        {                                       \
            .type = cccl_Token_##name,          \
            .value = code[i],                   \
        };                                      \
    } break
        case '/':
        {
            comment = 1;
        } break;
        case '\n':
        {
            comment = 0;
        } break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': 
        case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': 
        case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': 
        case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': 
        case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
            X(IDENTIFIER);
        case '^': case '+': case '-': case '*': case '~':
            X(COMMAND);
        case '%': case '=': case '!': case '$': case '&':
        case '<': case '>': case '#': case ':': case '@':
            X(COMMANDWITHARG);
        case '{': case '(': case '[': case '?':
            X(BLOCKSTART);
        case '}': case ')': case ']': case ';':
            X(BLOCKEND);
        case ' ': case '\t':
        { } break;
        default:
        {
            if (comment)
                break;
            errx(1, "Illegal symbol in a code: [%d] %c", code[i], code[i]);
        } break;
#undef X
        }
        assert(tokeni < tokens_length);
    } while(++i < size);

    return tokeni;
}
