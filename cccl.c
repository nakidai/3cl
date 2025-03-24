#include "cccl.h"

#include <stddef.h>
#include <stdio.h>


void cccl(struct cccl_File file)
{
    struct cccl_Token tokens[TOKENS_LIMIT];

    size_t tokens_amount = tokenize(file.buffer, file.size, tokens, TOKENS_LIMIT);
    printf("Read: %lu\n", tokens_amount);
}
