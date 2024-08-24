#include "3cl.h"
#include "readchar.h"

#include <stdbool.h>
#include <string.h>

#include "utils.h"


static const char *const space    = " \n\t";
static const char *const brackets = "{[(?;)]}";
static const char *const instr    = "^+-*~%=!$&<>@#:";
static const char *const alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

char ccl_readchar(struct CCL *ccl, struct CCLFrame *frame, enum CCLRCFlags flags)
{
    bool iscomment;

    char chr;
    for (;(chr = ccl->code[frame->ep]) != '\0'; ++frame->ep)
    {
        if (iscomment && chr == '\n')
            iscomment = false;
        if (iscomment)
            continue;
        if (chr == '/')
            iscomment = true;

        if (strchr(space, chr))
            continue;

        if (strchr(brackets, chr))
            if (!(flags & CCL_RC_BRACK) && flags & CCL_RC_DIE)
                goto invalid;
            else
                goto ok;
        else if (strchr(instr, chr))
            if (!(flags & CCL_RC_IS) && flags & CCL_RC_DIE)
                goto invalid;
            else
                goto ok;
        else if (strchr(alphabet, chr))
            if (!(flags & CCL_RC_ALP) && flags & CCL_RC_DIE)
                goto invalid;
            else
                goto ok;
        else if (chr == '_')
            if (!(flags & CCL_RC_US) && flags & CCL_RC_DIE)
                goto invalid;
            else
                goto ok;
        else
            goto invalid;
    }
ok:
    return chr;
invalid:
    die(1, "Invalid symbol at %d", frame->ep);
}
