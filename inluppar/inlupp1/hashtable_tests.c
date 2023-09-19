#include <CUnit/Basic.h>
#include "hashtable.h"
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
void test_create_destroy(void) 
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_once(void) 
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    int k = 1;
    // in this case k should not exist in h.
    if (ioopm_hash_table_lookup(h, k).success == false) 
    {
        char *v = "entry 1";
        ioopm_hash_table_insert(h, k, v);
    }

    if (ioopm_hash_table_lookup(h, k).success == true)
    {
        CU_ASSERT_EQUAL(ioopm_hash_table_lookup(h, k).value, "entry 1");
    }
    
    ioopm_hash_table_destroy(h);
}

void test_ht_value_NULL()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    ioopm_hash_table_insert(ht, 0, NULL);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, 0).success, true);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, 0).value, NULL);
    ioopm_hash_table_destroy(ht);
}

void test_negative_key()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    ioopm_hash_table_insert(h, -1, "test");
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(h, -1).success, true);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(h, -1).value, "test");
    ioopm_hash_table_destroy(h);
}

void test_lookup_empty()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   
   for (int i = 0; i < 18; ++i) 
    {
       CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, i).value);
    }
   CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, -1).value);
   ioopm_hash_table_destroy(ht);
}

void test_remove_multiple_entries()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    ioopm_hash_table_insert(h, 0, "hej");
    ioopm_hash_table_insert(h, 17, "apa");
    ioopm_hash_table_insert(h, 34, "hus");

    CU_ASSERT_EQUAL(ioopm_hash_table_remove(h, 51), NULL);
    CU_ASSERT_EQUAL(ioopm_hash_table_remove(h, 17), "apa");
    CU_ASSERT_EQUAL(ioopm_hash_table_remove(h, 0), "hej");
    CU_ASSERT_EQUAL(ioopm_hash_table_remove(h, 34), "hus");
    

    ioopm_hash_table_destroy(h);
}
void test_remove_single_entry()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    ioopm_hash_table_insert(h, 1, "val");
    CU_ASSERT_EQUAL(ioopm_hash_table_remove(h, 1), "val");
    ioopm_hash_table_destroy(h);
}

void test_remove_null_entry()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    CU_ASSERT_EQUAL(ioopm_hash_table_remove(h, 2), NULL);
    ioopm_hash_table_destroy(h);
}

void test_hash_table_size_is_one(void)
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    ioopm_hash_table_insert(h, 0, "hello");
    CU_ASSERT_EQUAL(ioopm_hash_table_size(h), 1);
    ioopm_hash_table_destroy(h);
}

void test_hash_table_size_is_empty(void)
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    CU_ASSERT_EQUAL(ioopm_hash_table_size(h), 0);
    ioopm_hash_table_destroy(h);
}

void test_hash_table_is_empty()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_clear()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    ioopm_hash_table_insert(h, 0, "a");
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_clear(h);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_clear_empty()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    ioopm_hash_table_clear(h);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_find_keys()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    int keys[6] = {42, 10, 0, 99, 100, -1};
    bool found[6] = {false};
    
    for (int i = 0; i < 6; i++)
    {
        ioopm_hash_table_insert(ht, keys[i], "wows!");
    }
    //int *ht_keys = ioopm_hash_table_keys(ht);
    int ht_keys[6] = {-1, 100, 99, 0, 10, 42};
    
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (ht_keys[i] == keys[j]) 
            {
                found[j] = true;
            }
        }
    }
    
    for (int i = 0; i < 6; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }
    
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_values()
{
    const arr_size = 5;
    ioopm_hash_table_t *h = ioopm_hash_table_create();
    int keys[arr_size] = {3, 4, 5, 6, 7};
    char *values[arr_size] = {"three", "four", "five", "six", "seven"};
    bool found[arr_size] = {false};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert(h, keys[i], values[i]);
    }
    


    int *hash_keys = ioopm_hash_table_keys(h);
    char **hash_values = ioopm_hash_table_values(h);
   
    
    for (int j = 0; j < arr_size; j++)
    {
        for (int k = 0; k < arr_size; k++)
        {
            if (values[j] == hash_values[k] && keys[j] == hash_keys[k])
            {
                found[j] = true;
            }
        }
        
    }

    for (int i = 0; i < arr_size; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }

    ioopm_hash_table_destroy(h);
}

// run the tests

int main() {

    //setup CUnit
    if (CU_initialize_registry() != CUE_SUCCESS) 
    {
        return CU_get_error();
    }


    CU_pSuite hashtable_test = CU_add_suite("hash table test", init_suite, clean_suite);
    if (hashtable_test == NULL) 
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
        (CU_add_test(hashtable_test, "hash table create and destroy", test_create_destroy) == NULL) ||
        (CU_add_test(hashtable_test, "insert into hashtable", test_insert_once) == NULL) ||
        (CU_add_test(hashtable_test, "lookup is empty", test_lookup_empty) == NULL) ||
        (CU_add_test(hashtable_test, "NULL is possible value", test_ht_value_NULL) == NULL) ||
        (CU_add_test(hashtable_test, "negative is possible key", test_negative_key) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_remove ]: adds and removes 3 entires correctly", test_remove_multiple_entries) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_remove ]: remove one entry from ht", test_remove_single_entry) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_remove ]: returning NULL from not valid entry to remove", test_remove_null_entry) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_size ]: returns size = 1 if only one entry", test_hash_table_size_is_one) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_size ]: returns size = 0 for no entries", test_hash_table_size_is_empty) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_is_empty ]: returns true for an empty hashtable", test_hash_table_is_empty) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_clear ]: clears a hashtable so that it's empty", test_hash_table_clear) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_clear ]: clears an empty hashtable correctly", test_hash_table_clear_empty) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_keys ]: find all keys", test_hash_table_find_keys) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_values ]: correctly extracts all values from the hashtable", test_hash_table_values) == NULL) ||
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