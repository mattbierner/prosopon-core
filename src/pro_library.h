#ifndef prosopon_core_pro_library
#define prosopon_core_pro_library

#include "prosopon/core.h"


typedef struct pro_library_list pro_library_list;


/**
 * @return A new library list.
 */
PRO_INTERNAL
pro_library_list* pro_library_list_new(pro_state_ref s, const char* file,
    pro_library_list* next);

/**
 * Has a library been loaded.
 *
 * @return 0 if false else true
 */
PRO_INTERNAL
int pro_library_loaded(pro_state_ref, const char* file);


PRO_INTERNAL
void pro_library_list_free(pro_state_ref, pro_library_list*);


#endif
