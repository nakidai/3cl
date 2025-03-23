#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


char *readfile(const char *name)
{
    char *code, chr;
    FILE *file = fopen(name, "r");
    if (errno) die(1, strerror(errno));
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        code = calloc(size + 1, sizeof(*code));

        for (int i = 0; i < size; ++i)
        {
            if ((chr = fgetc(file)) == EOF)
            {
                if (errno)
                    die(1, strerror(errno));
                else
                    break;
            }
            code[i] = chr;
        }
    fclose(file);

    return code;
}
