#include "cccl.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>


int verbose = 0;
int interactive = 0;
int dump = 0;

int main(int argc, char **argv)
{
    const char *name = *argv;

    int ch;
    while ((ch = getopt(argc, argv, "vid")) >= 0)
    {
        switch (ch)
        {
            case 'v':
            {
                verbose = 1;
            } break;
            case 'i':
            {
                interactive = 1;
            } break;
            case 'd':
            {
                dump = 1;
            } break;
            default:
            {
                fprintf(stderr, "usage: %s [-vid] file\n", name);
                exit(1);
            } break;
        }
    }
    argc -= optind;
    argv += optind;

    if (!*argv)
    {
        fprintf(stderr, "usage: %s [-vid] file\n", name);
        exit(1);
    }

    struct cccl_File file;

    int error = cccl_allocfile(*argv, &file);
    if (error)
        err(1, "cccl_readfile()");

    FILE *f = fopen(*argv, "r");
    if (!f)
        err(1, "fopen()");
    int bytes_read = fread(file.buffer, 1, file.size, f);
    if (ferror(f) || bytes_read != file.size)
        errx(1, "couldn't read %s", *argv);
    fclose(f);

    cccl(file);
}
