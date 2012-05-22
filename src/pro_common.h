#ifndef prosopon_pro_common
#define prosopon_pro_common

#include "prosopon/core.h"

#include <assert.h>


/**
 * Asserts a condition.
 *
 * Breaks if false in debug mode, else returns the resulting error. Should only
 * be used in PRO_API functions.
 */
#ifdef DEBUG
    #define PRO_API_ASSERT(cond, err) if (!(cond)){ assert(0); return (err); }
#else 
    #define PRO_API_ASSERT(cond, err) if (!(cond)) { return (err); } 
#endif

/**
 *
 */
#define PRO_API_ASSERT_STATE(s) PRO_API_ASSERT((s), PRO_INVALID_STATE)

/**
 * Asserts a reference is of a given type.
 *
 * Breaks if false in debug mode, else returns the resulting error. Should only
 * be used in PRO_API functions.
 */
#define PRO_API_ASSERT_TYPE(ref, type, err) {\
        pro_type found_type;\
        pro_error get_err = pro_get_type(s, ref, &found_type);\
        PRO_API_ASSERT(get_err == PRO_OK, (err));\
        PRO_API_ASSERT(found_type == (type), (err));\
    }   



#endif
