#include "cccl.h"

#include <stdlib.h>

#include <sys/stat.h>


int cccl_allocfile(const char *path, struct cccl_File *file)
{
    struct stat st;
    int err = stat(path, &st);
    if (err)
        return err;

    file->size = st.st_size;
    file->buffer = malloc(file->size);

    return !file->buffer;
}
