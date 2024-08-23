#include "3cl.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "instruction.h"


int ccl_init(struct CCL *ccl, const char *code, int (*in)(), void (*out)(int))
{
    *ccl = (struct CCL)
    {
        .code = code,
        .in = in,
        .out = out,
        .rootframe = (struct CCLFrame)
        {
            .prev = NULL, .next = NULL,
            .type = CCL_ROOT,
            .ep = 0,
            .vars = (struct CCLVariable)
            {
                .prev = NULL,
                .next = NULL,
                .name = '_',
                .value = 0
            },
        },
        .stack = (struct CCLStack)
        {
            .length = CCL_STACKSIZE,
            .stack = (CCLNum *)malloc(CCL_STACKSIZE)
        },
    };

    return errno;
}

void ccl_free(struct CCL *ccl)
{
    free(ccl->stack.stack);
    if (ccl->rootframe.next != NULL)
    {
        for (struct CCLFrame *frame = ccl->rootframe.next, *new;;)
        {
            if (frame->vars.next != NULL)
            {
                for (struct CCLVariable *var = frame->vars.next, *new;;)
                {
                    if (var->next == NULL)
                        break;
                    new = var->next;
                    free(var);
                    var = new;
                }
            }

            if (frame->next == NULL)
                break;
            new = frame->next;
            free(frame);
            frame = new;
        }
    }
}

void ccl_exec(struct CCL *ccl)
{
    struct CCLFrame *curframe = &ccl->rootframe;

    for (;;++curframe->ep)
        curframe = ccl_instruction(ccl, curframe);
}
