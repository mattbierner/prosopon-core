#ifndef prosopon_core_pro_common
#define prosopon_core_pro_common

#include "prosopon/core.h"

#ifdef DEBUG
#include <assert.h>
#endif


#pragma mark Assertions
/**
 * @section assertions Assertions
 *
 * API assertions break if false in debug mode, else returns the resulting error.
 * Should only be used in PRO_API functions.
 */

#ifdef DEBUG
    #define PRO_ASSERT(cond) assert((cond))
#else 
    #define PRO_ASSERT(cond)
#endif


/**
 * API assertion for a condition.
 */
#define PRO_API_ASSERT(cond, err) if (!(cond)){ PRO_ASSERT(0); return (err); }

/**
 * API assertion a given state is valid.
 */
#define PRO_API_ASSERT_STATE(s) PRO_API_ASSERT((s), PRO_INVALID_STATE)

/**
 * Asserts a reference is of a given type.
 */
#define PRO_API_ASSERT_TYPE(ref, type, err) {\
        pro_type found_type;\
        pro_error get_err = pro_get_type(s, ref, &found_type);\
        PRO_API_ASSERT(get_err == PRO_OK, (err));\
        PRO_API_ASSERT(found_type == (type), (err));\
    }   



#endif
