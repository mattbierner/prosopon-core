#ifndef prosopon_core_core
#define prosopon_core_core

/**
 * @file core.h
 * @brief Declares Prosopon-Core's core actor model functionality.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <prosopon/config.h>

#include <stddef.h>


#pragma mark Error

/**
 * Information about the error an API call generated.
 */
typedef enum
{
    PRO_OK = 0,
    /**<
     * The API call completed as expected
     */
    
    PRO_OUT_OF_MEMORY,
    /**<
     * The library failed to allocate more memory to complete the call.
     */
     
    PRO_LIBRARY_LOAD_ERROR,
    /**<
     * There was an error loading an Prosopon library.
     */
     
    PRO_INVALID_STATE,
    /**<
     * The referenced state is not valid.
     */
    
    PRO_INVALID_OPERATION,
    /**<
     * The requested operation is not valid.
     */
     
    PRO_BEHAVIOR_ERROR,
    /**<
     *
     */
     
    PRO_ERROR_MAX
} pro_error;



#pragma mark Structures

/**
 * An opaque reference to state of the prosopon machine.
 */
typedef struct pro_state* pro_state_ref;

/**
 * An opaque reference to an object.
 */
typedef struct pro_lookup* pro_ref;

/**
 * Null object reference.
 */
PRO_API_DATA pro_ref PRO_EMPTY_REF;


/**
 * Function representing the behavior of an actor.
 * 
 * @param t A lookup for self.
 * @param t The message being processed.
 * @param data Additional user defined data passed to the behavior.
 */
typedef void(pro_behavior)(pro_state_ref,
    pro_ref t, pro_ref msg, pro_ref ud);



#pragma mark Types

/**
 * The basic type of an object.
 */
typedef enum
{
    PRO_NONE_TYPE = 0,
    PRO_LIST_TYPE,
    PRO_ACTOR_TYPE,
    PRO_UD_TYPE,
    PRO_TYPE_MAX
} pro_type;

/**
 *
 */
typedef const char* pro_actor_type;
PRO_API_DATA pro_actor_type PRO_DEFAULT_ACTOR_TYPE;


#pragma mark State

/**
 * Function that implements memory allocation and deallocation.
 * 
 * Based on lua_Alloc and similar to realloc.
 *
 * @param ptr A pointer to memory allocated by the pro_alloc function to reallocate.
 *     If null, new memory is allocated.
 * @param size The size of the new memory block after reallocation. If zero,
 *     The memory is deallocated.
 * @param ud User data passed to the allocator.
 */
typedef void*(pro_alloc_impl)(void* ptr, size_t size, void* ud);


/**
 * Allocate and dellocate memory.
 *
 * @see pro_alloc_impl
 */
PRO_API
void* (pro_alloc) (pro_state_ref s, void* ptr, size_t size);

/**
 * Creates a new execution state.
 *
 * @param alloc Memory management function used to allocate and free all memory.
 * @param alloc_ud Userdata that is passed to the allocator.
 * @param path Array of paths to search for libraries.
 * @param[out] state The newly created state.
 *
 * @return Does not return PRO_INVALID_STATE
 *   PRO_OUT_OF_MEMORY if a new state cannot be allocated.
 */
PRO_API
pro_error (pro_state_create) (pro_alloc_impl* alloc, void* alloc_ud, const char** path,
    PRO_OUT pro_state_ref* state);

/**
 * Releases a state for future collection.
 */
PRO_API
pro_error (pro_state_release) (pro_state_ref);

/**
 * TODO: remove
 */
PRO_API
pro_error (pro_run) (pro_state_ref);

/**
 * @param[out] alloc The memory allocator used for the given state.
 */
PRO_API
pro_error (pro_get_alloc) (pro_state_ref,
    PRO_OUT pro_alloc_impl** alloc);

/**
 * Retains a reference for further use.
 *
 * @see memory_management.
 */
PRO_API
pro_error (pro_retain) (pro_state_ref, pro_ref);

/**
 * Release an referenced object for future collection.
 *
 * @see memory_management.
 */
PRO_API
pro_error (pro_release) (pro_state_ref, pro_ref);

/**
 * @param[out] type The type value of a lookup.
 */
PRO_API
pro_error (pro_get_type) (pro_state_ref,
    pro_ref lookup, PRO_OUT pro_type* type);

/**
 * Enum for status of a match operation.
 */
typedef enum 
{
    PRO_MATCH_FAIL = 0,
    /**<
     * The match failed and is complete.
     */
        
    PRO_MATCH_SUCCEED,
    /**<
     * The match succeed and is complete.
     */

    PRO_MATCH_CONTINUE
    /**
     * The match succeed and will continue.
     */
} pro_matching;

/**
 * Attempts to match two identifiers.
 *
 * @param[out] match One if the two items match else zero.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_OPERATION if the state is not valid or id is null.
 */
PRO_API
pro_error (pro_match) (pro_state_ref, pro_ref, pro_ref ref,
    PRO_OUT pro_matching* match);

/**
 * Gets the string representation of an object.
 *
 * @param[out] ud User data with a reference count of one to store the object.
 */
PRO_API
pro_error (pro_to_string) (pro_state_ref, pro_ref,
    PRO_OUT pro_ref* ud);


#pragma mark List

/**
 * Creates a new list.
 *
 * @param[out] msg A referance to the new list with a reference count of 1.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *   PRO_OUT_OF_MEMORY if memory allocation fails.
 */
PRO_API
pro_error (pro_list_create) (pro_state_ref,
    PRO_OUT pro_ref* msg);

/**
 * Get the number of objects in a list.
 *
 * @param[out] length The number of objects a list contains.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if msg does not reference a list.
 */
PRO_API
pro_error (pro_list_length) (pro_state_ref, pro_ref msg,
    PRO_OUT unsigned int* length);

/**
 * Get a value from a list.
 *
 * @param[out] result Referance at index or PRO_EMPTY_REF if out of bounds.
 *   Retains the refence.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE If msg does not reference a list.
 */
PRO_API
pro_error (pro_list_get) (pro_state_ref, pro_ref msg, unsigned int index,
    PRO_OUT pro_ref* result);

/**
 * Appends an value to a list to create a new list.
 *
 * Does not modify the origin list.
 *
 * @param[out] new_msg A reference to the new list with the new object appended
 *   with a reference count of 1. Should not be the same as msg, this will cause
 *   a memork leak.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if msg does not reference a list or value does
 *     not reference a valid object.
 */
PRO_API
pro_error (pro_list_append) (pro_state_ref, pro_ref msg, pro_ref value,
    PRO_OUT pro_ref* new_msg);


#pragma mark User Data

/**
 * Function called before a user data object is destroyed. 
 *
 * @param data A pointer to the allocated data.
 */
typedef void(pro_ud_deconstructor) (pro_state_ref, void* data);

/**
 * Default deconstructor called on user data.
 *
 * Frees the user data pointer.
 */
PRO_API
void (PRO_DEFAULT_UD_DECONSTRUCTOR) (pro_state_ref, void* data);

/**
 * Creates a new user data object with a given size and deconstructor.
 *
 * @param[out] out_ref A reference to the new user data object with a reference
 *   count of 1.
 * 
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *   PRO_OUT_OF_MEMORY If memory for the requested object cannot be allocated.
 */
PRO_API
pro_error (pro_ud_create) (pro_state_ref, size_t size, pro_ud_deconstructor* deconstructor,
    PRO_OUT pro_ref* out_ref);

/**
 * Requests access to user data data for reading.
 *
 * @param[out] out_data A pointer to the user data memory for reading. Must not 
 *   be written.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if ref is not user data.
 */
PRO_API
pro_error (pro_ud_read) (pro_state_ref, pro_ref,
    PRO_OUT const void** out_data);

/**
 * Requests access to user data data for writting.
 *
 * @param[out] out_data A pointer to the user data memory for both 
 *   writing and reading.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if ref is not a valid user data object.
 */
PRO_API
pro_error (pro_ud_write) (pro_state_ref, pro_ref,
    PRO_OUT void** out_data);


#pragma mark Actor

/**
 * Create a new actor.
 * 
 * @param type The actor type of the new actor. Determines primitive functions.
 * @param beh The behavior for the new actor. May be null.
 * @param data A reference to user data for the actor. May be PRO_EMPTY_REF.
 *
 * @param[out] out_ref The lookup for the new actor with a reference count of 1.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 */
PRO_API
pro_error (pro_actor_create) (pro_state_ref, pro_actor_type type,
    pro_behavior beh, pro_ref data,
    PRO_OUT pro_ref* out_ref);

/**
 * @param[out] out_type The actor type of a lookup.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if ref is not a valid actor.
 */
PRO_API
pro_error (pro_get_actor_type) (pro_state_ref, pro_ref,
    PRO_OUT pro_actor_type* out_type);

/**
 * Sends a message to an actor asynchronously.
 *
 * @param actor A reference to the actor that is sent the message or PRO_EMPTY_REF.
 *     PRO_EMPTY_REF acts as a message sink.
 * @param msg A reference to be sent to the actor. May be a reference for any type
 *   or PRO_EMPTY_TYPE.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if actor is not a valid actor.
 */
PRO_API
pro_error (pro_send) (pro_state_ref, pro_ref actor, pro_ref msg);

/**
 * Specify the behavior for an actor.
 *
 * Essentially sets up message forwarding from the old actor to the new actor.
 * Usually, 'become' forwards messages to a newly created actor but this is not
 * required.
 *
 * @param actor The actor we a specifying the new behavior for.
 * @param new_beh The actor that will recieve all messages from the old actor.
 *
 * @return
 *   PRO_OK if successful
 *   PRO_INVALID_STATE
 *    if either actor or new_beh are not valid actors.
 */
PRO_API
pro_error (pro_become) (pro_state_ref, pro_ref actor, pro_ref new_beh);



#ifdef __cplusplus
}
#endif

#endif
