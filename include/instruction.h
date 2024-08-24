#ifndef __CCL_INSTRUCTION_H__
#define __CCL_INSTRUCTION_H__

#include "3cl.h"

/**< Type for every instruction in 3cl */
typedef struct CCLFrame *(*CCLInstruction)(struct CCL *ccl, struct CCLFrame *frame);

/**
 * Execute next instruction
 * @param ccl CCL instance
 * @param frame Current frame
 * @return Updated frame
 */
struct CCLFrame *ccl_instruction(struct CCL *ccl, struct CCLFrame *frame);

#endif /* __CCL_INSTRUCTION_H__ */
