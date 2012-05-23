#ifndef prosopon_core_macros
#define prosopon_core_macros

#ifdef __cplusplus
extern "C" {
#endif

#include <prosopon/prosopon.h>
#include <prosopon/lib/std.h>

#include <string.h>


/** 
 * @file macros.h
 * @brief A collection of macros to ease performing common prosopon functions.
 */


/**
 * Is an actor of a given actor type?
 * 
 * @return 1 if true, else 0
 */
static inline
int pro_match_actor_type(pro_state_ref s, pro_ref t, pro_actor_type type)
{
    pro_actor_type actor_type;
    pro_get_actor_type(s, t, &actor_type);
    return !strcmp(type, actor_type);    
}

/**
 * Is an object of a given type?
 * 
 * @return 1 if true, else 0
 */
static inline
int pro_match_type(pro_state_ref s, pro_ref t, pro_type type)
{
    pro_type found_type;
    pro_get_type(s, t, &found_type);
    return type == found_type;    
}


/**
 * Appends an object to a list and returns the modified reference.
 */
static inline
void pro_list_append_inplace(pro_state_ref s, pro_ref* msg, pro_ref val)
{
    pro_ref new_list = PRO_EMPTY_REF;
    pro_list_append(s, *msg, val, &new_list);
    pro_release(s, *msg);
    *msg = new_list;
}


#ifdef __cplusplus
}
#endif

#endif