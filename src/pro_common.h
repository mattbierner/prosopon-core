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
 * Assertions break if false in debug mode, else returns the resulting error.
 * Should only be used in PRO_API functions.
 */


/**
 * Asserts a condition.
 */
#ifdef DEBUG
    #define PRO_API_ASSERT(cond, err) if (!(cond)){ assert(0); return (err); }
#else 
    #define PRO_API_ASSERT(cond, err) if (!(cond)) { return (err); } 
#endif

/**
 * Asserts that a given state is valid
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
