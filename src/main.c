#include "main.h"
#include "types.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>


const s8 *program_name;

static const s8 *const usage_message =
    "usage: %s [-h] file\n";
static const s8 *const usage_description =
    "ccl language interpreter\n"
    "Arguments:\n"
    "  file         file to execute\n"
    "Options\n"
    "  -h, --help   show this help message and quit\n";

static struct option long_options[] =
{
    {"help",     no_argument, NULL, 'b'},
    {0}
};

noreturn void usage(bool full)
{
    printf(usage_message, program_name);
    if (full) printf(usage_description);
    exit(full ? 0 : 1);
}

int main(i32 argc, s8 **argv)
{
    program_name = argv[0];

    i32 ch;
    while ((ch = getopt_long(argc, argv, "h", long_options, NULL)) != EOF)
    {
        switch (ch)
        {
        case 'h':
            usage(true);
            break;
        default:
            usage(false);
            break;
        }
    }
    if (argv[optind] == NULL)
        usage(false);

    return 0;
}
