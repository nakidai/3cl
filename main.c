#include "cccl.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    if (!argv[1])
        return 1;

    struct cccl_File file;

    int error = cccl_allocfile(argv[1], &file);
    if (error)
        err(1, "cccl_readfile()");

    FILE *f = fopen(argv[1], "r");
    if (!f)
        err(1, "fopen()");
    int bytes_read = 0;
    while (bytes_read < file.size)
    {
        int read_now = fread(
            file.buffer + bytes_read,
            sizeof(*file.buffer),
            (file.size - bytes_read) % 2048,
            f
        );
        if (read_now == 0)
        {
            if (ferror(f))
                errx(1, "couldn't read %s", argv[1]);
            else
                break;
        }
    }
    fclose(f);

    cccl(file);
}
