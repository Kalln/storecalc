#include <CUnit/Basic.h>
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct link link_t;
union elem
{
    char *str;
    int val;
    void *void_ptr;
};

struct link 
{
    elem_t val;
    link_t *next;
};

struct list 
{
    link_t *first;
    link_t *last;
    int size;
};

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

    // We have no elements in our list. Should be dummy objects
    CU_ASSERT_EQUAL(lt->last->val, NULL);
    CU_ASSERT_EQUAL(lt->first->val, NULL);
    
    // Append a value to the list. Should be the first and last element, since we only have one element. 
    ioopm_linked_list_append(lt, -1);
    //CU_ASSERT_PTR_NOT_NULL(lt->last);
    //CU_ASSERT_PTR_NOT_NULL(lt->last);
    CU_ASSERT_EQUAL(lt->first->val, NULL); 
    CU_ASSERT_EQUAL(lt->last->val, -1);
    CU_ASSERT_EQUAL(lt->first->next, lt->last); 
    
    // Append 100 to the list. This is now the last element. And first element should -1 not 100.
    ioopm_linked_list_append(lt, 100);
    CU_ASSERT_EQUAL(lt->last->val, 100);
    CU_ASSERT_EQUAL(lt->first->next->val, -1); 
    CU_ASSERT_NOT_EQUAL(lt->first->next, lt->last); // These should NOT be equal.
    CU_ASSERT_EQUAL(lt->first->next->next, lt->last); 
    CU_ASSERT_EQUAL(lt->last->next, NULL); // Last elements next should be NULL.

    ioopm_linked_list_destroy(lt);

}

void test_prepend_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
 
    ioopm_linked_list_prepend(lt, 10);
    ioopm_linked_list_prepend(lt, 40);

    // Our list looks like this: 40 -> 10 -> NULL.
    CU_ASSERT_EQUAL(lt->first->next->val, 40);
    CU_ASSERT_EQUAL(lt->first->next->next->val, 10);
    CU_ASSERT_EQUAL(lt->last->val, 10); 

    ioopm_linked_list_destroy(lt);
}

void test_get_value(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    // Insert 10 values (0-9).
    for (int i = 0; i < 10; i++)
    {
        ioopm_linked_list_append(lt, i);
    }

    // Check that values are correct.
    for (int i = 0; i < 10; i++)
    {
        CU_ASSERT_EQUAL(ioopm_linked_list_get(lt, i), i);
    }

    ioopm_linked_list_destroy(lt);
}

void test_insert_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    // Insert into list: 0->1->2->...->9->NULL.
    for (int i = 0; i < 10; i++)
    {
        ioopm_linked_list_append(lt, i);
    }

    ioopm_linked_list_insert(lt, 3, 100);
    // Now list looks like: 0->1->2->100->...->9->NULL.
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lt, 2), 2);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lt, 3), 100);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lt, 4), 3);

    ioopm_linked_list_destroy(lt);
}

void test_remove_elem_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    ioopm_linked_list_append(lt, 200);
    CU_ASSERT_TRUE(ioopm_linked_list_contains(lt, 200));
    ioopm_linked_list_remove(lt, 0);
    CU_ASSERT_FALSE(ioopm_linked_list_contains(lt, 200));

    ioopm_linked_list_destroy(lt);
}

void test_link_list_size(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    for (int i = 0; i < 10; i++)
    {
        ioopm_linked_list_append(lt, i);
    }
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lt), 10);

    ioopm_linked_list_destroy(lt);
}

void test_linked_list_empty(void)
{  
    ioopm_list_t *lt = ioopm_linked_list_create();

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lt));

    ioopm_linked_list_append(lt, 10);
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(lt));

    ioopm_linked_list_destroy(lt);
}

void test_clear_link_list(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    for (int i = 0; i < 10; i++)
    {
        ioopm_linked_list_append(lt, i);
    }
    ioopm_linked_list_clear(lt);

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lt));
    CU_ASSERT_PTR_NOT_NULL(lt->first);
    CU_ASSERT_PTR_NOT_NULL(lt->last);

    ioopm_linked_list_destroy(lt);
}

/**
 * 
 * PREDICATES FOLLOW FOR COMMING TESTS.
 * 
 * 
*/
bool all_values_are_less_then_10(int key, int *value, void *extra)
{
    return *value < 10;
}

bool any_value_is_x(int key, int *value, void *x)
{
    return *value == (int *) x;
}
/**
 * 
 * PREDICATES DONE FOR COMMING TESTS.
 * 
 * 
*/

void add_5_to_all_values(int key, int *value, void *extra)
{
    *value = *value + 5;
}

void test_linked_list_show_all(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    for (int i = 0; i < 5; i++)
    {
        ioopm_linked_list_append(lt, i);
    }
    
    CU_ASSERT_TRUE(ioopm_linked_list_all(lt, all_values_are_less_then_10, NULL));
    ioopm_linked_list_append(lt, 150);
    CU_ASSERT_FALSE(ioopm_linked_list_all(lt, all_values_are_less_then_10, NULL));

    ioopm_linked_list_destroy(lt);
}

void test_linked_list_any(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();

    CU_ASSERT_FALSE(ioopm_linked_list_any(lt, any_value_is_x, 10));

    for (int i = 8; i < 12; i++)
    {
        ioopm_linked_list_append(lt, i);
    }

    CU_ASSERT_TRUE(ioopm_linked_list_any(lt, any_value_is_x, 10));

    ioopm_linked_list_destroy(lt);
}

void test_link_list_apply_to_all(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    for (size_t i = 0; i < 5; i++)
    {
        ioopm_linked_list_append(lt, i);
    }

    CU_ASSERT_EQUAL(lt->last->val, 4);
    CU_ASSERT_EQUAL(lt->first->val, 0);

    ioopm_linked_list_apply_to_all(lt, add_5_to_all_values, NULL);
    
    CU_ASSERT_EQUAL(lt->last->val, 9);
    CU_ASSERT_EQUAL(lt->first->val, 5);
    
    ioopm_linked_list_destroy(lt);
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
        (CU_add_test(linked_list_test, "[ioopm_linked_list_prepend] linked list prepend", test_prepend_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_get] Gets the the right value", test_get_value) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_insert] insert into a linked list", test_insert_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_remove] removes a value", test_remove_elem_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_size] correctly returns size of a linked list", test_link_list_size) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_is_empty] returns true for an empty list", test_linked_list_empty) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_clear] correctly clears a list without freeing the list_pointer", test_clear_link_list) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_all] elements is true according to predicate", test_linked_list_show_all) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_any] atleast one element true according to predicate", test_linked_list_any) == NULL) ||
        (CU_add_test(linked_list_test, "[ioopm_linked_list_to_all] applied a function to all elements in list", test_link_list_apply_to_all) == NULL) ||
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