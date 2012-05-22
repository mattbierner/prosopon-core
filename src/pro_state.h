#ifndef prosopon_pro_state
#define prosopon_pro_state

#include "prosopon/core.h"

struct pro_message_queue;
struct pro_env_stack;
struct pro_library_list;
struct pro_actor_type_info_list;


/**
 * Structure that holds information about the state of the Prosopon machine
 */
typedef struct pro_state pro_state;
struct pro_state
{
    pro_alloc* alloc; /**< Allocator used to allocate and free all memory. */
    
    struct pro_library_list* libraries; /**< The set of loaded libraries. */
    
    struct pro_actor_type_info_list* actor_types; /**< The set of actor types. */
    
    struct pro_message_queue* message_queue; /**< The global message queue. */
    
    const char* const* path;
};



/**
 * @return The list of loaded libraries.
 */
PRO_INTERNAL
struct pro_library_list* pro_state_get_libraries(pro_state*);

/**
 *
 */
PRO_INTERNAL
void pro_state_set_libraries(pro_state*, struct pro_library_list* val);

PRO_INTERNAL
struct pro_actor_type_info_list* pro_state_get_actor_type_info(pro_state*);

PRO_INTERNAL
void pro_state_set_actor_type_info(pro_state*,
    struct pro_actor_type_info_list* val);

#endif
