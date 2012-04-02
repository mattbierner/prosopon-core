#ifndef prosopon_h
#define prosopon_h

#include "prosopon_config.h"

struct pro_env;


/**
 * @mainpage
 *
 * @author Matt Bierner
 * @version 0.1.0
 *
 * @section intro_sec Introduction
 */


/**
 * An opaque structure that holds information about the state of the prosopon
 * machine.
 */
typedef struct pro_state pro_state;

/**
 * An opaque structure used to identify environments.
 */
typedef struct pro_env pro_env_lookup; 

extern pro_env_lookup* PRO_EMPTY_ENV_LOOKUP;

/**
 * An opaque structure used to resolve identifiers.
 */
typedef struct pro_lookup pro_lookup;

extern pro_lookup* PRO_EMPTY_LOOKUP;

/**
 * A list of lookup values.
 */
typedef struct pro_lookup_list pro_lookup_list;
struct pro_lookup_list
{
    pro_lookup_list* next;
    pro_lookup* value;
};

/**
 * Function representing the behavior of an actor.
 * 
 * @param t A lookup for self.
 * @param data Additional user defined data passed to the behavior.
 */
typedef void(pro_behavior_impl)(pro_state*,
    const pro_lookup* t, const pro_lookup* msg, void* data);

typedef struct pro_behavior pro_behavior;
struct pro_behavior
{
    pro_behavior_impl* impl;
    void* data;
};

/**
 * Function implementing a constructor behavior
 *
 * @return A lookup to the constructed object or null if none.
 */
typedef pro_lookup*(pro_constructor_impl)(pro_state*,
    pro_lookup_list* arguments, void* data);

typedef struct pro_constructor pro_constructor;
struct pro_constructor
{
    pro_constructor_impl* impl;
    void* data;
};

#pragma mark Types

/**
 * The basic type of an object.
 */
typedef enum
{
    PRO_NONE_TYPE = 0,
    PRO_MESSAGE_TYPE,
    PRO_ACTOR_TYPE,
    PRO_CONSTRUCTOR_TYPE,
    PRO_TYPE_MAX
} pro_type;

typedef const char* pro_actor_type;
extern pro_actor_type PRO_DEFAULT_ACTOR_TYPE;

typedef int(pro_match_impl)(pro_state*,
    const pro_lookup* t, const void* tData,
    const pro_lookup* o, const void* oData);



typedef struct pro_actor_type_info pro_actor_type_info;
struct pro_actor_type_info
{
    pro_match_impl* match;
};

PRO_API void (pro_register_actor_type) (pro_state*,
    pro_actor_type, const pro_actor_type_info*);
    

#pragma mark State

/**
 * Creates a new execution state.
 *
 * @return The newly created state or null if error.
 */
PRO_API pro_state* (pro_state_create) (void);

/**
 * Releases a state for future collection.
 */
PRO_API void (pro_state_release) (pro_state*);

/**
 * @return The lookup for the current environment. 
 */
PRO_API pro_env_lookup* (pro_get_env) (pro_state*);

/**
 * Pushes an environment onto the environment stack
 */
PRO_API void (pro_push_env) (pro_state*, pro_env_lookup*);

/**
 * Pops an environment off the environment stack.
 */
PRO_API void (pro_pop_env) (pro_state*);


#pragma mark Environment

/**
 * Creates a new environment with a given parent.
 * 
 * @return The lookup for a newly created environment.
 */
PRO_API pro_env_lookup* (pro_env_create) (pro_state*, pro_env_lookup* parent);

/**
 * Release an environment for future collection.
 */
PRO_API void (pro_env_release) (pro_state*, pro_env_lookup*);

/**
 * @return The lookup for the highest resolved lookup for a given name.
 */
PRO_API pro_lookup* (pro_get_binding) (pro_state*,
    pro_env_lookup* env, const char* name);

/**
 * @return The type value of a lookup.
 */
PRO_API pro_type (pro_get_type) (pro_state*,
    const pro_lookup* lookup);

/**
 * Binds a lookup to an identifier name.
 */
PRO_API void (pro_bind) (pro_state*, pro_lookup* lookup, const char* id);

/**
 * 
 */
PRO_API int (pro_match)(pro_state*,
    const pro_lookup* l1, const pro_lookup* l2);

/**
 * 
 */
PRO_API const char* (pro_to_string)(pro_state*,
    const pro_lookup*);

#pragma mark Constructor

/**
 * Creates a new constructor in the current environment.
 *
 * @param data Additional user defined data passed to the constructor.
 *             @see pro_constructor
 *
 * @return The lookup for the new constructor.
 */
PRO_API pro_lookup* (pro_constructor_create) (pro_state*,
    pro_constructor);

/**
 * Calls a constructor with a list of arguments.
 * 
 * @param constructor The lookup for the constructor.
 * @param arguments A list of lookups to pass to the constructor.
 *
 * @return The result from the constructor.
 */
PRO_API pro_lookup* (pro_constructor_call) (pro_state*,
    pro_lookup* constructor, pro_lookup_list* arguments);


#pragma mark Message

/**
 * Creates a new message in the current environment.
 *
 * @return The lookup for the new message.
 */
PRO_API pro_lookup* (pro_message_create) (pro_state*);

/**
 * @return The number of objects a message contains.
 */
PRO_API unsigned int (pro_message_length) (pro_state*,
    const pro_lookup* lookup);

/**
 * Get a value from a message.
 *
 * @return lookup or null if out of bounds.
 */
PRO_API pro_lookup* (pro_message_get) (pro_state*,
    const pro_lookup* lookup, unsigned int idx);

/**
 * Appends an value to a message.
 */
PRO_API void (pro_message_append) (pro_state*,
    const pro_lookup* msg, pro_lookup* lookup);


#pragma mark Actor

/**
 * Create a new actor object in the current environment.
 *
 * @return The lookup for the new actor.
 */
PRO_API pro_lookup* (pro_actor_create) (pro_state*, pro_actor_type type);

/**
 * @return The type value of a lookup.
 */
PRO_API pro_actor_type (pro_get_actor_type) (pro_state*, const pro_lookup* lookup);

/**
 * Sends a message to an actor.
 */
PRO_API void (pro_send) (pro_state*,
    const pro_lookup* actor, const pro_lookup* msg);

/**
 * Specify the behavior for an actor.
 */
PRO_API void (pro_become) (pro_state*, pro_lookup* actor, pro_behavior);


#pragma mark Library Loading

#ifdef PRO_DYNAMIC_LIBRARY_LOADING

typedef void(pro_library_init)(pro_state*);

/** 
 *
 */
PRO_API void (pro_library_load) (pro_state*, const char* file);

#endif


#endif
