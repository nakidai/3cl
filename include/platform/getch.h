#ifndef __GETCH_H__
#define __GETCH_H__

#ifdef _WIN32
#include <conio.h>
#define getch _getche
#else
int getch(void);
#endif /* _WIN32 */

void getch_init(void);

#endif /* __GETCH_H__ */
