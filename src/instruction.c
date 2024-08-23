#include "instruction.h"

#include "3cl.h"

struct CCLFrame *ccl_instruction(struct CCL *ccl, struct CCLFrame *frame)
{
    CCLInstruction instruction;
    switch (ccl->code[frame->ep])
    {
    case '\n': /* FALLTHROUGH */
    case ' ' : /* FALLTHROUGH */
    case '\t': instruction = ccl_instruction_nop;
    case '^' : instruction = ccl_instruction_pushzero;
    case '+' : instruction = ccl_instruction_increment;
    case '-' : instruction = ccl_instruction_decrement;
    }
    return instruction(ccl, frame);
}
