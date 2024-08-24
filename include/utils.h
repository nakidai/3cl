#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdnoreturn.h>


/**
 * Show message:
 * program_name: <formattedstring>\n
 * and exit with code
 * @param code Code to exit
 * @param fmt Format string
 */
noreturn void die(int code, char *fmt, ...);

#endif /* __UTILS_H__ */
