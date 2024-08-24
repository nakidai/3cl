#include "3cl.h"

#include "readchar.h"
#include "stack.h"
#include "utils.h"

struct CCLFrame *ccl_instruction_add(struct CCL *ccl, struct CCLFrame *frame)
{
    if (ccl->stack.cur < 2)
        die(1, "stack size is %d (%d required)", ccl->stack.cur, 2);
    ccl_stack_push(&ccl->stack, ccl_stack_pop(&ccl->stack) + ccl_stack_pop(&ccl->stack));

    return frame;
}
