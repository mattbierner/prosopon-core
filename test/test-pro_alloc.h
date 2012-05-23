#ifndef prosopon_test_pro_alloc
#define prosopon_test_pro_alloc

#include <stddef.h>


typedef struct test_alloc_ud test_alloc_ud;

extern test_alloc_ud* test_alloc_ud_create();

void* test_alloc(void* ptr, size_t nsize, void* ud);

void* test_bad_alloc(void* ptd, size_t nsize, void* ud);




#endif
