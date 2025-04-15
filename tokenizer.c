#include "cccl.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stddef.h>


enum CodeStatus
{
    Code_CODE = 0,
    Code_SINGLELINE_COMMENT,
    Code_MULTILINE_COMMENT,
};

size_t cccl_tokenize(const char *code, size_t size, struct cccl_Token tokens[], size_t tokens_length)
{
    size_t i = 0, tokeni = 0;
    enum CodeStatus status = Code_CODE;

    if (!code || !size)
        return 0;

    do
    {
        switch (code[i])
        {
#define X(name)                                 \
    {                                           \
        if (status != Code_CODE)                \
            break;                              \
        tokens[tokeni++] = (struct cccl_Token)  \
        {                                       \
            .type = cccl_Token_##name,          \
            .value = code[i],                   \
        };                                      \
    } break
        case '/':
        {
            if (status == Code_MULTILINE_COMMENT)
                break;
            status = Code_SINGLELINE_COMMENT;
        } break;
        case '\n':
        {
            if (status == Code_MULTILINE_COMMENT)
                break;
            status = Code_CODE;
        } break;
        case '\\':
        {
            if (status == Code_SINGLELINE_COMMENT)
                break;
            status = status == Code_MULTILINE_COMMENT
                ? Code_CODE
                : Code_MULTILINE_COMMENT;
        } break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': 
        case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': 
        case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': 
        case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': 
        case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
            X(IDENTIFIER);
        case '^': case '+': case '-': case '*': case '~': case '#': case ':':
            X(COMMAND);
        case '%': case '=': case '!': case '$':
        case '&': case '<': case '>': case '@':
            X(COMMANDWITHARG);
        case '{': case '(': case '[': case '?':
            X(BLOCKSTART);
        case '}': case ')': case ']': case ';':
            X(BLOCKEND);
        case ' ': case '\t':
        { } break;
        default:
        {
            if (status != Code_CODE)
                break;
            errx(1, "Illegal symbol in a code at byte %lu: [%d] %c", i, code[i], code[i]);
        } break;
#undef X
        }
        if (tokeni >= tokens_length)
            errx(1, "Exceeded limit of %lu tokens", tokens_length);
    } while(++i < size);

    return tokeni;
}
