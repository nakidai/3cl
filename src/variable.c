#include "3cl.h"
#include "variable.h"

#include <stdbool.h>
#include <stdlib.h>


struct CCLVariable *ccl_variable_get(struct CCLVariable *vars, char name)
{
    for (struct CCLVariable *var = vars;;var = var->next)
        if (var->name == name)
            return var;
        else if (var->name == '_' || var->next == NULL)
            return NULL;
}

struct CCLVariable *ccl_variable_getany(struct CCL *ccl, struct CCLFrame *frame, char name)
{
    bool root = false;
    struct CCLFrame *curframe;
    struct CCLVariable *found;

    for (curframe = frame; curframe->type != CCL_PROC && curframe->type != CCL_ROOT; curframe = curframe->prev);
    if (curframe->type == CCL_ROOT)
        root = true;

    found = ccl_variable_get(&curframe->vars, name);
    if (!found && !root)
        return ccl_variable_get(&ccl->rootframe.vars, name);

    return NULL;
}
