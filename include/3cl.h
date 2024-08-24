#ifndef __3CL_H__
#define __3CL_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifndef CCL_STACKSIZE
#define CCL_STACKSIZE 8192
#endif

typedef int16_t CCLNum;   /**< Abstract type for numbers in ccl */
typedef size_t  CCLAddr;  /**< Abstract type for address in ccl */

/**
 * Type of CCLFrame
 */
enum CCLFrameType
{
    CCL_ROOT,    /**< Root frame, stores global variables    */
    CCL_PROC,    /**< Function frame, stores local variables */
    CCL_INFLOOP, /**< Infinity loop, stores start and repeat */
    CCL_REPLOOP, /**< Repeat loop, stores start and repeat   */
};

/**
 * Pair which contains variable's name and value
 */
struct CCLVariable
{
    struct CCLVariable *prev, *next;
    char                name;
    CCLNum              value;
};

/**
 * Pair which contains procedure's name and address
 */
struct CCLProcedure
{
    struct CCLProcedure *prev, *next;
    char                 name;
    CCLAddr              address;
};

/**
 * Pair with length of stack and stack itself
 */
struct CCLStack
{
    size_t  length;
    size_t  cur;
    CCLNum *stack;
};

/**
 * Linked list that stores current state and allows to do recursion with local variables.
 */
struct CCLFrame
{
    struct  CCLFrame      *prev, *next;  /**< Frames are connected as linked list */
    enum    CCLFrameType   type;         /**< Type of the frame                   */
    CCLAddr                ep;           /**< Execution point                     */
    union
    {
        /* Frame stores either variables (for procedures)... */
        struct
        {
            struct  CCLVariable vars;    /**< For root frame variables are global */
        };
        /* ...or start address and repeats left (for loops) */
        struct
        {
            CCLAddr start;               /**< Used by CCL_*LOOP                   */
            int     repeats;             /**< Used by CCL_REPLOOP                 */
        };
    };
};

/**
 * Main structure for 3cl. It contains root frame, stack and code it's executing.
 */
struct CCL
{
    struct CCLStack   stack;
    struct CCLFrame   rootframe;
    const char       *code;
    bool              stopped;
    int             (*in)();
    void            (*out)(int);
};

/**
 * Inits CCL.
 * @see ccl_free
 * @see ccl_exec
 * @param ccl The structure to init
 * @param code The code to execute
 * @param in Function to get input
 * @param out Function to show output
 * @return 0 on success, any other number otherwise
 */
int ccl_init(struct CCL *ccl, const char *code, int (*in)(), void (*out)(int));

/**
 * Frees all things that ware allocated in ccl_init.
 * @see ccl_init
 * @param ccl The structure to free
 */
void ccl_free(struct CCL *ccl);

/**
 * Executes inited CCL instance
 * @see ccl_init
 * @param ccl The strcture to execute
 */
void ccl_exec(struct CCL *ccl);

#endif /* __3CL_H__ */
