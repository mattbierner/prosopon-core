#include "pro_env.h"

#include "pro_state.h"
#include "pro_common.h"
#include "pro_object.h"
#include "pro_lookup.h"
#include "pro_env_lookup.h"
#include "pro_lookup_table.h"
#include "pro_binding_map.h"

#pragma mark PRO_INTERNAL

PRO_INTERNAL pro_env* pro_env_new(pro_state_ref s,
    pro_env_ref parent, unsigned int ref_count)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    pro_env* e = alloc(0, sizeof(*e));
    if (!e) return 0;
    
    e->parent = parent;
    e->ref_count = ref_count;
    e->lookups = pro_lookup_table_new(s);
    e->bindings = pro_binding_map_new(s);
    return e;
}


PRO_INTERNAL void pro_env_free(pro_state_ref s, pro_env* t)
{
    pro_alloc* alloc;
    pro_get_alloc(s, &alloc);
    
    pro_binding_map_free(s, t->bindings);    
    pro_lookup_table_free(s, t->lookups);
    
    pro_env_release(s, t->parent);
    
    alloc(t, 0);
}


PRO_INTERNAL pro_ref pro_env_next_lookup(pro_state_ref s,
    pro_env_ref env_ref)
{
    pro_env_retain(s, env_ref);
    pro_env* env = pro_env_dereference(s, env_ref);
    
    unsigned int index = pro_lookup_table_append(s, env->lookups);
    return pro_lookup_new(s, env_ref, index, 1);
}


PRO_INTERNAL pro_object** pro_env_lookup_value(pro_state_ref s, pro_ref ref)
{
    pro_env* env = pro_env_dereference(s, ref->env);
    return pro_lookup_table_get(s, env->lookups, ref->index);
}


PRO_INTERNAL pro_object* pro_dereference(pro_state_ref s, pro_ref ref)
{
    pro_env* env = pro_env_dereference(s, ref->env);
    return *pro_lookup_table_get(s, env->lookups, ref->index);
}

PRO_INTERNAL pro_env* pro_internal_env_retain(pro_state_ref s, pro_env* env)
{
    (env->ref_count)++;
    return env;
}


PRO_INTERNAL void pro_internal_env_release(pro_state_ref s, pro_env* env)
{
    if (--(env->ref_count) == 0)
        pro_env_free(s, env);
}


PRO_INTERNAL void pro_env_lookup_remove(pro_state_ref s, pro_env* env, pro_ref ref)
{
    pro_lookup_table_remove(s, env->lookups, ref->index);
}


#pragma mark -
#pragma mark PRO_API

PRO_API pro_error pro_env_create(pro_state_ref s, pro_env_ref parent,
    PRO_OUT pro_env_ref* env_out)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    
    pro_env_retain(s, parent);
    pro_env* env = pro_env_new(s, parent, 1);
    PRO_API_ASSERT(env, PRO_OUT_OF_MEMORY);

    pro_env_lookup* env_lookup = pro_env_lookup_new(s, env, 1);
    PRO_API_ASSERT(env_lookup, PRO_OUT_OF_MEMORY);
    
    *env_out = env_lookup; 
    return PRO_OK;
}


PRO_API pro_error pro_bind(pro_state_ref s, pro_ref ref, const char* id)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_REF != ref, PRO_INVALID_ARGUMENT);
    PRO_API_ASSERT(id, PRO_INVALID_OPERATION);

    pro_retain(s, ref);

    pro_env_ref env_ref;
    pro_get_env(s, &env_ref);
    pro_env* env = pro_env_dereference(s, env_ref);

    pro_binding_map_put(s, env->bindings, id, ref);
    
    pro_env_release(s, env_ref);

    return PRO_OK;
}


PRO_API pro_error pro_get_binding(pro_state_ref s,
    pro_env_ref env_ref, const char* name,  PRO_OUT pro_ref* ref)
{
    PRO_API_ASSERT(s, PRO_INVALID_OPERATION);
    PRO_API_ASSERT(PRO_EMPTY_ENV_REF != env_ref, PRO_INVALID_ARGUMENT);

    pro_env* env = pro_env_dereference(s, env_ref);
    
    pro_ref val = pro_binding_map_get(s, env->bindings, name);
    if (!pro_lookup_equal(s, val, PRO_EMPTY_REF))
    {
        *ref = val;
        return PRO_OK;
    }
    
    if (env->parent)
        return pro_get_binding(s, env->parent, name, ref);
    else
    {
        *ref = PRO_EMPTY_REF;
        return PRO_OK;
    }
}

