#include "3cl.h"

#include "stack.h"

struct CCLFrame *ccl_instruction_pushzero(struct CCL *ccl, struct CCLFrame *frame)
{
    ccl_stack_push(&ccl->stack, 0);
    return frame;
}
