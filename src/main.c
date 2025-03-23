#include "main.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include "platform/getch.h"
#include "3cl.h"
#include "readfile.h"


const char *program_name;

static const char *const usage_message =
    "usage: %s [-h] file\n";
static const char *const usage_description =
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

int main(int argc, char **argv)
{
    program_name = argv[0];
    getch_init();

    int ch;
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

    char *code = readfile(argv[optind]);
    struct CCL ccl;
    ccl_init(&ccl, code, getch, (void(*)(int))putchar);
    ccl_exec(&ccl);
    free(code);
    ccl_free(&ccl);
    return 0;
}
