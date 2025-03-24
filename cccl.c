#include "cccl.h"

#include <stddef.h>
#include <stdio.h>


void cccl(struct cccl_File file)
{
    struct cccl_Token tokens[TOKENS_LIMIT];

    size_t tokens_amount = cccl_tokenize(file.buffer, file.size, tokens, TOKENS_LIMIT);
    if (verbose)
        fprintf(stderr, "Read: %lu\n", tokens_amount);
    struct cccl_Node *parsed = cccl_parse(tokens, tokens_amount, 0, 0);
}
