#ifndef __CCCL_H__
#define __CCCL_H__

#include "types.h"

#include <stdbool.h>

#include "cvector.h"


typedef struct cccl_varpair
{
    i16 value;
    s8  name;
} cccl_varpair;

typedef struct cccl_procpair
{
    i32 value;
    s8  name;
} cccl_procpair;

typedef struct cccl_pointer
{
    i32  value;
    bool is_loop;
} cccl_pointer;

typedef struct cccl
{
    cccl_varpair   *variables;  /* Array with variables      */
    cccl_procpair  *procedures; /* Array with procedures     */
    i16            *stack;      /* User stack                */
    s8             *br_stack;   /* Stack for brackets        */
    i32            *ep_stack;   /* Call stack                */
    i32             ep;         /* Pointer to current symbol */
    s8             *code;       /* Code being executed       */
    s8             *filename;   /* File being executed       */
    i32             size;       /* File size                 */
} cccl;

void cccl_init(s8 *filename);
void cccl_read(void);
void cccl_run(void);
void cccl_free(void);

i32 cccl_readnext(void);

#endif /* __CCCL_H__ */
