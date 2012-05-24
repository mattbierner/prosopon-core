#ifndef prosopon_core_prosopon
#define prosopon_core_prosopon

/**
 * @file prosopon.h
 * @brief Main Prosopon-Core header. Declares all Prosopon-Core functionality.
 */
 
/**
 * @mainpage
 * @author Matt Bierner
 * @version 0.2.0
 *
 * Prosopon is an embeddable actor model based programming language written in C.
 * \n
 * The Prosopon-Core library provides support the core actor model functionality
 * of Prosopon.
 * 
 *
 * @section public_headers Public Headers
 *
 * @li prosopon.h Declares all Prosopon-Core functionality.
 *
 * @li config.h Declares build and runtime configuration for Prosopon-Core.
 *
 * @li core.h Declares Prosopon-Core's core actor model functionality.
 *
 * @li actor_type.h Declares Prosopon-Core functionality to define actor types.
 *
 * @li library.h Declares Prosopon-Core functionality to load Prosopon libraries.
 *
 * @li library.h Defines some common Prosopon-Core functions.
 * 
 *
 * @section error_handeling Error Handling
 *
 * Unless otherwise documented, every API call returns an error code to report
 * problems. PRO_OK is returned when no errors occur.
 *
 * It is the programmer's responsibilty to check the returned error codes and
 * handle any error appropriately. Failure to handle an error for one call
 * may effect later calls and the exected behavior of the program.
 *
 * Unless otherwise documented, each API call returns PRO_OK if successful or
 * PRO_INVALID_STATE if the state was not valid.
 *
 *
 * @section memory_sec Memory Management
 *
 * @subsection alloc_dealloc Allocation and Deallocation
 * All memory managed by Prosopon must be allocated and deallocated using
 * the pro_alloc function.
 * Additionally, it is strongly recommended that any memory used in the system,
 * even if not directly managed by Prosopon, also be allocated and deallocated
 * using pro_alloc.
 *
 * @subsection reference_counting Reference Counting
 * All Prosopon objects (pro_state_ref, pro_env_ref, pro_ref), are memory 
 * managed using manual reference counting. It is the programmer's responsibility
 * to correctly manage memory in order to prevent memory leaks and memory
 * access bugs.
 *
 * API functions involving memory management make not of how they change reference
 * counts or the reference count of objects they return.
 *
 * The following general points are good guidelines for manageing memory: 
 * @li Any object that will be used further must be retained. Many API functions
 *   return retained objects.
 * @li Any object you are done using should be released.
 * @li Every retaining statement should have a matching release. Returned parementers
 *   must always be released if not used.
 * @li Release != Free, memory may not be freed when release is called even if the
 *   reference count is zero. It will always eventually be freed.
 */

 
#include <prosopon/core.h>
#include <prosopon/actor_type.h>
#include <prosopon/library.h>



#endif
