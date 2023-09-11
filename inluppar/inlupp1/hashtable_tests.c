#include <CUnit/Basic.h>
#include "hashtable.h"

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// Own tests here.
void test_create_destroy(void) {
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_once(void) {
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    int k = 1;
    if (ioopm_hash_table_lookup(h, k) == NULL) {
        char *v = "entry 1";
        ioopm_hash_table_insert(h, k, v);
    }
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(h, 1), "entry 1");

    ioopm_hash_table_destroy(h);
}
// run the tests

int main() {

    //setup CUnit
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }


    CU_pSuite my_test_suite = CU_add_suite("hash table test suite", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
        (CU_add_test(my_test_suite, "hash table create and destroy", test_create_destroy) == NULL) ||
        (CU_add_test(my_test_suite, "insert into hashtable", test_insert_once) == NULL) ||
        0
    ) 
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();

    return CU_get_error();
}