#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdnoreturn.h>

#include "types.h"


noreturn void die(i32 code, s8 *fmt, ...);

#endif /* __UTILS_H__ */
