#ifndef prosopon_core_library
#define prosopon_core_library

#ifdef __cplusplus
extern "C" {
#endif

#include <prosopon/core.h>


/**
 * @file library.h
 * @brief Support for loading Prosopon libraries at runtime.
 */
 
/**
 * @section prosopon-libraries Prosopon Libraries
 *
 *
 */


#pragma mark -
#pragma mark Library

/**
 * Initilization function called when a Prosopon library is loaded.
 */
typedef void(prosopon_library_initilization)(pro_state_ref);


/** 
 * Loads and initilizes a Prosopon library.
 *
 * @param name The name of the library to load. Must be the library name, such 
 *   as "std", not the name of the library file.
 * @param lib_name The name of the library file to load. Must not include the
 *   extension
 * @param handle[out]
 *
 * @return 
 *   PRO_LIBRARY_LOAD_ERROR If the requested library cannot be found or it does
 *     not provide a valid initilization function.
 */
PRO_API
pro_error (pro_library_load) (pro_state_ref,
    const char* name, const char* lib_name, PRO_OUT void** handle);


#define PRO_LIBRARY_INIT_FUNCTION(NAME) prosopon_library_init_ ## NAME

/**
 * Declares a Prosopon library.
 */
#define PRO_LIBRARY(NAME) \
    extern PRO_VISIBILITY_PUBLIC prosopon_library_initilization* PRO_LIBRARY_INIT_FUNCTION(NAME)

/**
 * Registers a Prosopon library.
 */
#define PRO_REGISTER_LIBRARY(NAME, INIT) \
    prosopon_library_initilization* PRO_LIBRARY_INIT_FUNCTION(NAME) = (INIT)


#ifdef __cplusplus
}
#endif

#endif
