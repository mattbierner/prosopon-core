#include "test-pro_alloc.h"

#include <stdlib.h>


struct test_alloc_ud
{
    size_t allocated;
};

extern test_alloc_ud* test_alloc_ud_create()
{
    test_alloc_ud* t = malloc(sizeof(*t));
    t->allocated = 0;
    return t;
}


void* test_alloc(void* ptr, size_t nsize, void* ud)
{
    test_alloc_ud* data = ud;
    
    if (0 == nsize)
    {
        if (!ptr)
            return 0;
        
        size_t* o_ptr = ptr;
        
        size_t old_size = *((size_t*)o_ptr);
        data->allocated -= old_size;
        
        free(o_ptr - 1);
        return 0;
    }
    else
    {
        if (ptr != 0)
        {
            size_t* o_ptr = ptr;
            data->allocated -= *(o_ptr - 1);
        }
        
        void* mem = realloc(ptr, nsize + sizeof(size_t));
        size_t* size = mem;
        size[0] = nsize;
        data->allocated += nsize;

        return &size[1];
    }
}


void* test_bad_alloc(void* ptd, size_t nsize, void* ud)
{
    return 0; 
}
