#include "3cl.h"

#include "utils.h"

struct CCLFrame *ccl_instruction_invalid(struct CCL *ccl, struct CCLFrame *frame)
{
    die(1, "Invalid instruction at %d", frame->ep - 1);
}
