#include <CUnit/Basic.h>
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int init_suite(void) 
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) 
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// Own tests here.

void test_create_empty_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    CU_ASSERT_PTR_NOT_NULL(lt);
    ioopm_linked_list_destroy(lt);
}

void test_append_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    CU_ASSERT_FALSE( ioopm_linked_list_contains(lt, -1));
    ioopm_linked_list_append(lt, -1);
    CU_ASSERT_TRUE( ioopm_linked_list_contains(lt, -1));
    ioopm_linked_list_destroy(lt);
}

void test_prepend_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    
    ioopm_linked_list_prepend(lt, 10);
    // Hur kontrollera sista?
    ioopm_linked_list_prepend(lt, 40);
    // Hur kontrollera sista? 

    ioopm_linked_list_destroy(lt);
}

void test_insert_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    ioopm_linked_list_insert(lt, 0, 1);
    CU_ASSERT_TRUE( ioopm_linked_list_contains(lt, -1));
    ioopm_linked_list_destroy(lt);
}

void test_remove_elem_link_list(void)
{
    
}

void test_get_elem_by_index_link_list(void)
{

}

void test_elem_contains_in_link_list(void)
{

}

void test_link_list_size(void)
{

}

void test_linked_list_empty(void)
{

}

void test_clear_link_list(void)
{

}

void test_linked_list_show_all(void)
{

}

void test_linked_list_any(void)
{

}

void test_link_list_apply_to_all(void)
{
    
}

// run the tests

int main() {

    //setup CUnit
    if (CU_initialize_registry() != CUE_SUCCESS) 
    {
        return CU_get_error();
    }


    CU_pSuite linked_list_test = CU_add_suite("Linked list test", init_suite, clean_suite);
    if (linked_list_test == NULL) 
    {
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
        (CU_add_test(linked_list_test, "[ioopm_linked_list_[create / destroy]] linked list create and destroy", test_create_empty_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_append] linked list append", test_append_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_prepend]linked list prepend", test_prepend_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_insert]linked list create and destroy", test_insert_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_remove]linked list create and destroy", test_remove_elem_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_get]linked list create and destroy", test_get_elem_by_index_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_contains]linked list create and destroy", test_elem_contains_in_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_size]linked list create and destroy", test_link_list_size) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_is_empty] linked list create and destroy", test_linked_list_empty) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_clear] linked list create and destroy", test_clear_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_all] linked list create and destroy", test_linked_list_show_all) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_any] linked list create and destroy", test_linked_list_any) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_to_all] linked list create and destroy", test_link_list_apply_to_all) == NULL) ||

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