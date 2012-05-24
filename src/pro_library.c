#include "pro_library.h"

#include "prosopon/library.h"

#include "pro_state.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>


#pragma mark Private

struct pro_library_list
{
    pro_library_list* next;
    const char* file;
    void* handle;
};

static int initialize_library(pro_state_ref s, const char* name, void* lib_handle)
{
    assert(lib_handle);
    
    char init_sym[256];
    sprintf(init_sym, "prosopon_library_init_%s", name);
    
    union { void* d; prosopon_library_initilization** f; } u;
    u.d = dlsym(lib_handle, init_sym);
    prosopon_library_initilization** init = u.f;
    if (!init)
        return 0;
    
    (*init)(s);
    return 1;
}

static void add_loaded_library(pro_state_ref s, const char* file, void* handle)
{
    pro_library_list* libraries = pro_state_get_libraries(s);
    pro_state_set_libraries(s, pro_library_list_new(s, file, libraries));
}

static char* get_library_filename(const char* name)
{
    char* buf = malloc(sizeof(*buf) * 256);
    sprintf(buf, "%s.dylib", name);
    return buf;
}

static void* open_library(pro_state_ref s, const char* filename)
{
    if (s->path == 0)
        return 0;
        
    unsigned int i = 0;
    for (const char* dirname = s->path[0]; dirname; dirname = s->path[++i])
    {
        DIR* dir = opendir(dirname);
        if (!dir) continue;
        
        for (struct dirent* dent; (dent = readdir(dir)); )
        {
            char path[PATH_MAX + 1];
            snprintf(path, PATH_MAX + 1, "%s/%s", dirname, filename);
            if (access(path, F_OK) == 0)
                return dlopen(path, RTLD_LOCAL | RTLD_LAZY);
        }
    }

    return 0;
}

#pragma mark -
#pragma mark Internal

PRO_INTERNAL
pro_library_list* pro_library_list_new(pro_state_ref s,
    const char* file, pro_library_list* next)
{
    pro_library_list* t = pro_alloc(s, 0, sizeof(*t));
    if (!t) return 0;
    
    t->next = next;
    t->file = file;
    return t;
}


PRO_INTERNAL
int pro_library_loaded(pro_state_ref s, const char* file)
{
    for (pro_library_list* library = pro_state_get_libraries(s); library; library = library->next)
        if (strcmp(file, library->file) == 0)
            return 1;
    return 0;
}

PRO_INTERNAL
void pro_library_list_free(pro_state_ref s, pro_library_list* libraries)
{
    for (pro_library_list* lib = libraries; lib; )
    {
        pro_library_list* next = lib->next;
        dlclose(lib->handle);
        pro_alloc(s, lib, 0);
        lib = next;
    }
}



#pragma mark PRO_API

PRO_API
pro_error pro_library_load(pro_state_ref s,
    const char* name, const char* lib_name, PRO_OUT void** out_handle)
{
    if (pro_library_loaded(s, name) != 0)
        return PRO_OK;
    
    pro_error err = PRO_OK;
    char* filename = get_library_filename(lib_name);
    
    void* handle = open_library(s, filename);
    if (handle)
    {
        if (!initialize_library(s, name, handle))
            err = PRO_LIBRARY_LOAD_ERROR;
        else
            add_loaded_library(s, name, handle);
    }
    else
        err = PRO_LIBRARY_LOAD_ERROR;
    
    free(filename);
    *out_handle = handle;
    return err;
}
