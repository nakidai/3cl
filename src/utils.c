#include "utils.h"
#include "types.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include "main.h"


noreturn void die(i32 code, s8 *fmt, ...)
{
    va_list args;

    fprintf(stderr, "%s: ", program_name);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    putc('\n', stderr);

    exit(code);
}
