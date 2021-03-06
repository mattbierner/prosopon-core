#include "prosopon/core.h"

#include "pro_common.h"
#include "pro_object.h"
#include "pro_lookup.h"


PRO_API
pro_error pro_constructor_create(pro_state_ref s,
    pro_constructor* c, pro_ref ud, PRO_OUT pro_ref* constructor)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT(c, PRO_INVALID_OPERATION);

    // Get the current environment
    pro_env_ref current_env;
    pro_get_env(s, &current_env);
    
    // Create a new object
    pro_object* obj = pro_object_new(s, PRO_CONSTRUCTOR_TYPE, 1);
    PRO_API_ASSERT(obj, PRO_OUT_OF_MEMORY);
    
    // Set the implementation
    obj->value.constructor.constructor = c;
   
    // Set the user data
    pro_retain(s, ud);
    obj->value.constructor.data = ud;
    
    // Set the environment
    obj->value.constructor.env = current_env;
    
    // Return the result
    pro_ref lookup = pro_lookup_new(s, obj, 1);
    PRO_API_ASSERT(lookup, PRO_OUT_OF_MEMORY);

    *constructor = lookup;
    return PRO_OK;
}


PRO_API
pro_error pro_constructor_call(pro_state_ref s,
    pro_ref constructor, pro_ref arguments, PRO_OUT pro_ref* result)
{
    PRO_API_ASSERT_STATE(s);
    PRO_API_ASSERT_TYPE(constructor, PRO_CONSTRUCTOR_TYPE, PRO_INVALID_OPERATION);
    
    pro_object* obj = pro_dereference(s, constructor);
    
    // Create a new env for the call with the parent from the save constructor
    {
        pro_env_ref env;
        pro_env_create(s, obj->value.constructor.env, &env);
        pro_push_env(s, env);
        pro_env_release(s, env);
    }
    
    // Call the constructor implementation
    pro_constructor* impl = obj->value.constructor.constructor;
    pro_ref ud = obj->value.constructor.data;
    pro_ref out = impl(s, arguments, ud);
    
    // Error if constructor returns nothing
    PRO_API_ASSERT(!pro_lookup_equal(s, out, PRO_EMPTY_REF), PRO_CONSTRUCTOR_ERROR);
    
    // Release the env
    pro_pop_env(s);
    
    *result = out;
    return PRO_OK;
}

