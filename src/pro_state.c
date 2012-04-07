#include "pro_state.h"

#include "prosopon.h"
#include "pro_type.h"
#include "pro_common.h"
#include "pro_messaging.h"
#include "pro_env_stack.h"
#include "pro_message_queue.h"
#include "pro_env.h"
#include "pro_lookup.h"

#include <stdlib.h>


#pragma mark Private

static pro_state* pro_state_new(pro_global_state* g)
{
    pro_state_ref s = malloc(sizeof(*s));
    if (!s) return 0;
    s->global = g;
    return s;
}

static pro_global_state* pro_global_state_new()
{
    pro_global_state* g = malloc(sizeof(*g));
    if (!g) return 0;
    
    pro_state* s = pro_state_new(g);
    if (!s)
    {
        free(g);
        return 0;
    }
    
    g->main = s;
    g->libraries = 0;
    g->message_queue = pro_message_queue_new(s);

    return g;
}


#pragma mark -
#pragma mark Intenal

PRO_INTERNAL struct pro_library_list* pro_state_get_libraries(pro_state* s)
{
    return s->global->libraries;
}

PRO_INTERNAL void pro_state_set_libraries(pro_state* s,
    struct pro_library_list* val)
{
    s->global->libraries = val;
}

PRO_INTERNAL struct pro_actor_type_info_list* pro_state_get_actor_type_info(pro_state* s)
{
    return s->global->actor_types;
}

PRO_INTERNAL void pro_state_set_actor_type_info(pro_state* s,
    struct pro_actor_type_info_list* val)
{
    s->global->actor_types = val;
}


#pragma mark -
#pragma mark PRO_API

PRO_API pro_error pro_state_create(PRO_OUT pro_state_ref* out_state)
{
    pro_global_state* g = pro_global_state_new();
    PRO_API_ASSERT(g, PRO_OUT_OF_MEMORY);
    
    pro_state* s = g->main;
    
    pro_env_ref root_env = pro_env_lookup_new(s, pro_env_new(s, 0));
    PRO_API_ASSERT(root_env, PRO_OUT_OF_MEMORY);
    pro_env_stack* stack = pro_env_stack_new(s);
    pro_env_stack_push(s, stack, root_env);
    PRO_API_ASSERT(stack, PRO_OUT_OF_MEMORY);

    s->root_env = root_env;
    s->stack = stack;
    
    initialize_default_actor_types(s);
    
    
    *out_state = s;
    return PRO_OK;
}

PRO_API pro_error pro_state_release(pro_state_ref s)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    
    // relase all environments
    while (!pro_env_lookup_equal(s, pro_env_stack_top(s, s->stack), s->root_env))  
        pro_pop_env(s); 
    
    pro_env_release(s, s->root_env);
    
    free(s); // free state memory
    return PRO_OK;
}


PRO_API pro_error pro_run(pro_state_ref s)
{
    while (!pro_message_queue_is_empty(s, s->global->message_queue))
    {
        pro_ref actor;
        pro_ref msg = pro_message_queue_dequeue(s, s->global->message_queue, &actor);
        pro_deliver_message(s, actor, msg);
    }
    return PRO_OK;
}


PRO_API pro_error pro_get_env(pro_state_ref s, PRO_OUT pro_env_ref* out_env)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    *out_env = pro_env_lookup_new(s, pro_env_stack_top(s, s->stack)->value);
    return PRO_OK;
}


PRO_API pro_error pro_push_env(pro_state_ref s, pro_env_ref env)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_ENV_REF != env, PRO_INVALID_ARGUMENT);
    pro_env_ref current_env = pro_env_stack_top(s, s->stack);
    // error if pushing env onto self.
    PRO_API_ASSERT(!pro_env_lookup_equal(s, env, current_env), PRO_INVALID_OPERATION);
    
    pro_env_stack_push(s, s->stack, env);
    return PRO_OK;
}


PRO_API pro_error pro_pop_env(pro_state_ref s)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    // Dont allow popping the root.
    PRO_API_ASSERT(!(pro_env_lookup_equal(s, pro_env_stack_top(s, s->stack), s->root_env)),
        PRO_INVALID_OPERATION);
    
    pro_env_ref old = pro_env_stack_top(s, s->stack);
    pro_env_stack_pop(s, s->stack);
    
    // Free previous stack entry.
    pro_env_release(s, old);
    
    return PRO_OK;
}
