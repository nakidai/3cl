#include "3cl.h"

#include <stdlib.h>


static inline void resize(struct CCLStack *stack)
{
    stack->length *= 2;
    stack->stack = realloc(stack->stack, stack->length * sizeof(*stack->stack));
}

void ccl_stack_push(struct CCLStack *stack, CCLNum num)
{
    if (stack->cur + 1 == stack->length)
        resize(stack);
    stack->stack[++stack->cur] = num;
}

CCLNum ccl_stack_pop(struct CCLStack *stack, CCLNum num)
{
    return stack->stack[stack->cur--];
}
