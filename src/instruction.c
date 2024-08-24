#include "instruction.h"
#include "instructions.h"

#include <stdbool.h>

#include "readchar.h"
#include "3cl.h"


struct CCLFrame *ccl_instruction(struct CCL *ccl, struct CCLFrame *frame)
{
    CCLInstruction instruction;
    char chr = ccl_readchar(ccl, frame, CCL_RC_CCL_INSTR);

    if (chr == '\0')
    {
        ccl->stopped = true;
        return INST(nop)(ccl, frame);
    }

#define ISSW(NAME) instruction = INST(NAME); break
    switch (chr)
    {
    case '\n': /* FALLTHROUGH */
    case ' ' : /* FALLTHROUGH */
    case '\t': ISSW(nop);
    case '^' : ISSW(pushzero);
    case '+' : ISSW(increment);
    case '-' : ISSW(decrement);
    case '*' : ISSW(add);
    case '~' : ISSW(subtract);
    case '%' : ISSW(reverse);
    }
#undef INSW
    return instruction(ccl, frame);
}
