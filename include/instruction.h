#ifndef __CCL_INSTRUCTION_H__
#define __CCL_INSTRUCTION_H__

#include "3cl.h"


typedef struct CCLFrame *(*CCLInstruction)(struct CCL *ccl, struct CCLFrame *frame);

struct CCLFrame *ccl_instruction_nop(struct CCL *ccl, struct CCLFrame *frame);
struct CCLFrame *ccl_instruction_pushzero(struct CCL *ccl, struct CCLFrame *frame);
struct CCLFrame *ccl_instruction_increment(struct CCL *ccl, struct CCLFrame *frame);
struct CCLFrame *ccl_instruction_decrement(struct CCL *ccl, struct CCLFrame *frame);

struct CCLFrame *ccl_instruction(struct CCL *ccl, struct CCLFrame *frame);

#endif /* __CCL_INSTRUCTION_H__ */
