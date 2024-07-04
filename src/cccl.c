#include "cccl.h"
#include "types.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "cvector.h"
#include "utils.h"


static cccl pc = {0};

void cccl_init(s8 *filename)
{
    memset(&pc, 0, sizeof(pc));

    cvector_init(pc.stack, 1, NULL);
    cvector_init(pc.variables, 1, NULL);
    cvector_init(pc.procedures, 1, NULL);
    cvector_init(pc.ep_stack, 1, NULL);
    if (errno) die(1, strerror(errno));

    pc.filename = filename;
}

void cccl_free()
{
    cvector_free(pc.stack);
    cvector_free(pc.variables);
    cvector_free(pc.procedures);
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

void cccl_run(void)
{
    bool is_comment;
    for (pc.ep = 0; pc.ep < pc.size; ++pc.ep)
    {
        if (is_comment && pc.code[pc.ep] == '\n')
            is_comment = false;
        switch (pc.code[pc.ep])
        {
        case '/':
            is_comment = true;
            break;
        }
    }
}
