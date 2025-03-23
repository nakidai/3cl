#ifndef __CCL_INSTRUCTIONS_H__
#define __CCL_INSTRUCTIONS_H__

#include "3cl.h"


#define INST(NAME) \
    struct CCLFrame *ccl_instruction_##NAME(struct CCL *ccl, struct CCLFrame *frame)

INST(nop);
INST(pushzero);
INST(increment);
INST(decrement);
INST(add);
INST(subtract);
INST(reverse);
INST(assign);
INST(invalid);

#undef INST
#define INST(NAME) ccl_instruction_##NAME

#endif /* __CCL_INSTRUCTIONS_H__ */
