#ifndef __CCL_READCHAR_H__
#define __CCL_READCHAR_H__

#include "3cl.h"


/**
 * Flags to tell ccl_readchar how to read next character
 *
 * Bits
 * 4 Die if char is not whitelisted
 * 3 Brackets
 * 2 Instruction symbols
 * 1 English alphabet
 * 0 Underscore
 *
 * @see ccl_readchar
 */
enum CCLRCFlags
{
    CCL_RC_DIE   = 0b10000, /**< Die if char is not whitelisted */
    CCL_RC_BRACK = 0b01000, /**< Brackes                        */
    CCL_RC_IS    = 0b00100, /**< Instruction symbols            */
    CCL_RC_ALP   = 0b00010, /**< English alphabet               */
    CCL_RC_US    = 0b00001, /**< Underscore                     */
    CCL_RC_VAR       = 0b11010, /**< (Used by 3CL) Variable names                      */
    CCL_RC_CCL_VARUS = 0b11011, /**< (Used by 3CL) Variable names including underscore */
    CCL_RC_CCL_BRACK = 0b01000, /**< (Used by 3CL) Brackets                            */
    CCL_RC_CCL_INSTR = 0b11111, /**< (Used by 3CL) Instruction                         */
};

/**
 * Function to read next character
 * Skips whitespace and comments, returns '\0' if reaches EOF
 * ccl_readchar will die on invalid symbol (e.g. cyrillic one) even if CCL_RC_DIE is set
 * @see CCLRCFlags
 * @param ccl CCL instance
 * @param frame Current frame
 * @param flags CCLRCFlags
 * @return Next character
 */
char ccl_readchar(struct CCL *ccl, struct CCLFrame *frame, enum CCLRCFlags flags);

#endif /* __CCL_READCHAR_H__ */
