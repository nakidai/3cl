#include "3cl.h"
#include "instruction.h"

struct CCLFrame *ccl_instruction_nop(struct CCL *ccl, struct CCLFrame *frame)
{
    return frame;
}
