#ifndef __OPERATION_H__
#define __OPERATION_H__

#include "cccl.h"


typedef void (*cccl_operation)(void);

void cccl_operation_init(cccl *pc);
cccl_operation cccl_select_operation(void);

#endif /* __OPERATION_H__ */
