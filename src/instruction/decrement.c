#include "3cl.h"
#include "instruction.h"

struct CCLFrame *ccl_instruction_decrement(struct CCL *ccl, struct CCLFrame *frame)
{
    --ccl->stack.stack[ccl->stack.cur];
    return frame;
}
