#include "test.h"

#include "prosopon/core.h"
#include "pro_state.h"
#include "pro_lookup.h"

#include "test-pro_alloc.h"


static int init(void)
{
    return 0;
}

static int cleanup(void)
{

    return 0;
}


#pragma mark -
#pragma mark Tests


static void test_create(void)
{
    pro_state_ref s;
    pro_state_create(test_alloc, test_alloc_ud_create(), 0, &s);
    CU_ASSERT(0 != s);
}

static void test_create_bad_alloc(void)
{
    pro_state_ref s;
    pro_error err = pro_state_create(test_bad_alloc, 0, 0, &s);
    CU_ASSERT(PRO_OUT_OF_MEMORY == err);
}


static void test_release_invalid(void)
{
    pro_error err = pro_state_release(0);
    CU_ASSERT(PRO_INVALID_STATE == err);
}


static CU_TestInfo tests[] = {
    {"create", test_create},
    {"create_bad_alloc", test_create_bad_alloc},
    {"release invalid", test_release_invalid},
    CU_TEST_INFO_NULL,
};


CU_SuiteInfo pro_state_test_suite = {
    "pro_state",
    init,
    cleanup,
    tests
};
