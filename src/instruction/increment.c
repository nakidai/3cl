#include "3cl.h"

#include "utils.h"

struct CCLFrame *ccl_instruction_increment(struct CCL *ccl, struct CCLFrame *frame)
{
    if (ccl->stack.cur < 1)
        die(1, "stack size is %d (%d required)", ccl->stack.cur, 1);
    ++ccl->stack.stack[ccl->stack.cur];
    return frame;
}
