#ifndef __CCL_VARIABLE_H__
#define __CCL_VARIABLE_H__

#include "3cl.h"


/**
 * Tries to find variable in list, NULL if not found.
 * @see ccl_variable_getany
 * @param vars Variable list
 * @param name Variable name
 */
struct CCLVariable *ccl_variable_get(struct CCLVariable *vars, char name);

/**
 * Tries to find variable in current frame, then in root, NULL if not found.
 * @see ccl_variable_get
 * @param ccl CCL instance
 * @param frame Current frame
 * @param name Variable name
 */
struct CCLVariable *ccl_variable_getany(struct CCL *ccl, struct CCLFrame *frame, char name);

#endif /* __CCL_VARIABLE_H__ */