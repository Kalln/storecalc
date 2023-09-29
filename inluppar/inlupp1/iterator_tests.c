#include <CUnit/Basic.h>
#include "iterator.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct elem elem_t;
struct elem 
{
    int val;
    elem_t *next;
};
struct ioopm_list_iterator
{
    ioopm_list_t *list;
    elem_t **current;
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

void test_create_iter(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    ioopm_linked_list_append(lt, 2);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lt);
    
    CU_ASSERT_PTR_NOT_NULL(iter);
    elem_t *elem = *(iter->current);
    CU_ASSERT_EQUAL(elem->val, 2);
    
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(lt);
}

void test_iter_has_next(void)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));
    ioopm_linked_list_append(list, 2);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));
    ioopm_linked_list_append(list, 3);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
    
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_next(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    for (size_t i = 0; i < 3; i++)
    {
        ioopm_linked_list_append(lt, i);
    }
    
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lt);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter), 1);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter), 2);
    
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(lt);
}

void test_iter_remove(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lt);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(lt);
}

void test_iter_insert(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lt);
    
    for (int i = 0; i < 10; i++)
    {
        ioopm_linked_list_append(lt, i);
    }
    
    ioopm_iterator_next(iter);
    ioopm_iterator_insert(iter, 200); // hade lt istället för iter, fungerar nu

    CU_ASSERT_EQUAL(ioopm_linked_list_get(lt, 1), 200);
    
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(lt);
}

void test_iter_reset(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    for (size_t i = 0; i < 3; i++)
    {
        ioopm_linked_list_append(lt, i);
    }
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lt);
    CU_ASSERT_EQUAL((*(iter->current))->val, 0);
    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL((*(iter->current))->val, 1);
    
    ioopm_iterator_reset(iter); // Vi förde med lt istället för iter
    
    CU_ASSERT_EQUAL((*(iter->current))->val, 0);
    
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(lt);
}

void test_current(void)
{
    ioopm_list_t *lt = ioopm_linked_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lt);
    for (size_t i = 0; i < 5; i++)
    {
        ioopm_linked_list_append(lt, 5*i);
    }
    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter), 5);

    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter), 10);

    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter), 15);
    
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(lt);
}
// run the tests

int main() {

    //setup CUnit
    if (CU_initialize_registry() != CUE_SUCCESS) 
    {
        return CU_get_error();
    }


    CU_pSuite iterator_test_suite = CU_add_suite("iterator test suite", init_suite, clean_suite);
    if (iterator_test_suite == NULL) 
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
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator] Create and destroy an iterator", test_create_iter) == NULL) ||
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator_has_next] Tests that iterator correctly tells if next is not NULL", test_iter_has_next) == NULL) ||
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator_next] Checks that iterator correctly jumps to the next value if there is one", test_iter_next) == NULL) ||
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator_next] Loops through and checks multiple values", test_current) == NULL) ||
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator_remove] Removes an existing value", test_iter_remove) == NULL) ||
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator_insert] Inserts and updates pointers", test_iter_insert) == NULL) ||
        (CU_add_test(iterator_test_suite, "[ioopm_list_iterator_reset] Resets the pointer to the start of the list", test_iter_reset) == NULL) ||
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