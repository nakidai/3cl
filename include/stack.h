#ifndef __CCL_STACK_H__
#define __CCL_STACK_H__

#include "3cl.h"

void ccl_stack_push(struct CCLStack *stack, CCLNum num);
CCLNum ccl_stack_pop(struct CCLStack *stack);

#endif /* __CCL_STACK_H__ */
