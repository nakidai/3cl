#ifndef __CCL_STACK_H__
#define __CCL_STACK_H__

#include "3cl.h"


/**
 * Push num to the stack
 * @param stack Stack where to push
 * @param num Number to push
 */
void ccl_stack_push(struct CCLStack *stack, CCLNum num);

/**
 * Pop number from the stack
 * @param stack Stack from where to pop
 * @return Popped number
 */
CCLNum ccl_stack_pop(struct CCLStack *stack);

#endif /* __CCL_STACK_H__ */
