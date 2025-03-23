#include "3cl.h"

#include <stdio.h>

#include "readchar.h"
#include "stack.h"
#include "utils.h"
#include "variable.h"


struct CCLFrame *ccl_instruction_assign(struct CCL *ccl, struct CCLFrame *frame)
{
    if (ccl->stack.cur < 1)
        die(1, "stack size is %d (%d required)", ccl->stack.cur, 1);

    CCLNum value = ccl_stack_pop(&ccl->stack);

    char name = ccl_readchar(ccl, frame, CCL_RC_CCL_VARUS);
    if (name == '\0')
        die(1, "Unexpected EOF");

    if (name == '_')
        return frame;

    struct CCLVariable *var = ccl_variable_getany(ccl, frame, name);
    if (var == NULL)
        ccl_variable_set(&ccl->rootframe.vars, name, value);
    else
        var->value = value;

    return frame;
}
