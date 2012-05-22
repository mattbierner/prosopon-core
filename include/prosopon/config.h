#ifndef prosopon_core_config
#define prosopon_core_config

/** 
 * @file config.h
 * @brief Configuration file for prosopon-core build and runtime.
 */

#if __llvm__ || __GNUC__ >= 4
    #define PRO_VISIBILITY_PUBLIC __attribute__ ((visibility ("default")))
    #define PRO_VISIBILITY_PRIVATE __attribute__ ((visibility ("hidden")))
#else
    #define PRO_VISIBILITY_PUBLIC
    #define PRO_VISIBILITY_PRIVATE
#endif


#pragma mark -
#pragma Symbols

/**
 * Marks a symbol as part of the public prosopon API. 
 */
#define PRO_API	extern PRO_VISIBILITY_PUBLIC

/**
 * Marks a data symbol as part of the public prosopon API. 
 */
#define PRO_API_DATA extern PRO_VISIBILITY_PUBLIC

/**
 * Marks a symbol as part of the public prosopon core library API. 
 */
#define PRO_LIBCORE extern PRO_VISIBILITY_PUBLIC

/**
 * Marks a symbol as internal, not intended for public use.
 */
#define PRO_INTERNAL extern PRO_VISIBILITY_PRIVATE


#endif
