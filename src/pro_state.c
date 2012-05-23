#include "pro_state.h"

#include "pro_type.h"
#include "pro_common.h"
#include "pro_messaging.h"
#include "pro_message_queue.h"
#include "pro_lookup.h"
#include "pro_library.h"


#pragma mark Private

static pro_state* pro_state_new(pro_alloc* alloc)
{
    pro_state* s = alloc(0, sizeof(*s));
    if (!s) return 0;
    
    s->alloc = alloc;

    s->libraries = 0;
    s->message_queue = pro_message_queue_new(s);
    s->actor_types = 0;
    
    return s;
}


static void pro_state_free(pro_state* s)
{
    pro_alloc* alloc = s->alloc;
    
    if (s->libraries)
        pro_library_list_free(s, s->libraries);
    if (s->actor_types)
        pro_actor_type_info_list_free(s, s->actor_types);
    if (s->message_queue)
        pro_message_queue_free(s, s->message_queue);
    
    // Free global state structure memory
    alloc(s, 0);
}


#pragma mark -
#pragma mark Internal

PRO_INTERNAL
struct pro_library_list* pro_state_get_libraries(pro_state* s)
{
    return s->libraries;
}

PRO_INTERNAL
void pro_state_set_libraries(pro_state* s,
    struct pro_library_list* val)
{
    s->libraries = val;
}

PRO_INTERNAL
struct pro_actor_type_info_list* pro_state_get_actor_type_info(pro_state* s)
{
    return s->actor_types;
}

PRO_INTERNAL
void pro_state_set_actor_type_info(pro_state* s,
    struct pro_actor_type_info_list* val)
{
    s->actor_types = val;
}


#pragma mark -
#pragma mark Public

PRO_API
pro_error pro_state_create(pro_alloc* alloc, const char** path,
    PRO_OUT pro_state_ref* out_state)
{
    // Allocate and setup the global state
    pro_state* s = pro_state_new(alloc);
    PRO_API_ASSERT(s, PRO_OUT_OF_MEMORY);
    
    s->path = path;
    initialize_default_actor_types(s);
    
    *out_state = s;
    return PRO_OK;
}


PRO_API
pro_error pro_state_release(pro_state_ref s)
{
    PRO_API_ASSERT_STATE(s);
    
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    // Free global state
    pro_state_free(s);
    
    // Free state structure memory
    //alloc(s, 0);
    
    return PRO_OK;
}


PRO_API
pro_error pro_get_alloc(pro_state_ref s, PRO_OUT pro_alloc** alloc)
{
    *alloc = s->alloc;
    return PRO_OK;
}


PRO_API
pro_error pro_run(pro_state_ref s)
{
    pro_state_ref exec_state = s;

    while (!pro_message_queue_is_empty(s, s->message_queue))
    {    
        pro_ref actor;
        pro_ref msg = pro_message_queue_dequeue(s, s->message_queue, &actor);
        pro_deliver_message(exec_state, actor, msg);
        pro_release(exec_state, msg);
        pro_release(exec_state, actor);
    }
    return PRO_OK;
}
