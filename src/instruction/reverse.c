#include "3cl.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readchar.h"
#include "utils.h"
#include "variable.h"


struct CCLFrame *ccl_instruction_reverse(struct CCL *ccl, struct CCLFrame *frame)
{
    char varname;
    size_t toreverse;

    varname = ccl_readchar(ccl, frame, CCL_RC_CCL_VARUS);
    if (varname == '\0')
    {
        die(1, "Unexpected EOF");
    } else if (varname == '_')
    {
        toreverse = ccl->stack.cur;
    } else
    {
        struct CCLVariable *var = ccl_variable_getany(ccl, frame, varname);
        if (var == NULL)
            die(1, "Unknown variable '%c' at %d", varname, frame->ep);
        toreverse = var->value;
    }

    if (ccl->stack.cur < toreverse)
        die(1, "stack size is %d (%d required)", ccl->stack.cur, toreverse);

    CCLNum *temp = malloc(sizeof(*temp) * toreverse);
    if (temp == NULL)
        die(1, "malloc(): %s", strerror(errno));

    size_t start;
    for (start = 0; start < toreverse; ++start)
        temp[start] = ccl->stack.stack[ccl->stack.cur - start];
    start = ccl->stack.cur - start;
    for (int i = 0; i < toreverse; ++i)
        ccl->stack.stack[start + i] = temp[i];

    free(temp);

    return frame;
}
