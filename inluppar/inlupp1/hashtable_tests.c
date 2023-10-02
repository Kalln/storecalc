#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "hashtable.h"
#include "iterator.h"
#include "common.h"
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
// Example hash:
int example_hash_function(elem_t elem)
{
    if (elem.type == ELEM_STR)
    {
        return strlen(elem.data.str);
    }
    return elem.data.val > 0 ? elem.data.val : 0;
}

int example_hash_function_str(elem_t elem)
{
    return strlen(elem.data.str);
}





// Own tests here.
void test_create_destroy(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, int_eq);
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}



void test_insert_once(void)
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, int_eq);
    elem_t k = int_elem(1);
    // in this case k should not exist in h.
    if (ioopm_hash_table_lookup(h, k).success == false)
    {
        elem_t v = ptr_elem("entry 1");
        ioopm_hash_table_insert(h, k, v);
    }

    if (ioopm_hash_table_lookup(h, k).success == true)
    {
        CU_ASSERT_TRUE(str_eq(ioopm_hash_table_lookup(h, k).value, ptr_elem("entry 1")));

        // The change of allocating memory to each value, made this fail.
        // I have no idea why?
        // CU_ASSERT_EQUAL(ioopm_hash_table_lookup(h, k).value, "entry 1");
    }

    ioopm_hash_table_destroy(h);
}

void test_ht_value_NULL()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, int_eq);
    ioopm_hash_table_insert(ht, int_elem(0), void_elem(NULL));
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, int_elem(0)).success, true);
    //CU_ASSERT_TRUE(int_eq(ioopm_hash_table_lookup(ht, 0).value, int_elem(NULL)));
    ioopm_hash_table_destroy(ht);
}

void test_negative_key()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, int_eq);

    ioopm_hash_table_insert(h, int_elem(-1), ptr_elem("test"));

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(h, int_elem(-1)).success);
    CU_ASSERT_TRUE(int_eq(ioopm_hash_table_lookup(h, int_elem(-1)).value, ptr_elem("test")));

    ioopm_hash_table_destroy(h);
}

void test_lookup_empty()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, str_eq);

    // TODO: REWRITE THESE TESTS
    for (int i = 0; i < 18; ++i)
    {
        //CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, void_elem(i)).value);
    }
    //CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(-1)).value);
    ioopm_hash_table_destroy(ht);
}

void test_remove_multiple_entries()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);

/*     elem_t k1 = int_elem(0);
    elem_t k2 = int_elem(17);
    elem_t k3 = int_elem(34); */
    elem_t k1 = ptr_elem("");
    elem_t k2 = ptr_elem("hej");
    elem_t k3  = ptr_elem("hejhejhej");

    elem_t v1 = ptr_elem("hej");
    elem_t v2 = ptr_elem("apa");
    elem_t v3 = ptr_elem("hus");
    elem_t v4 = void_elem(NULL);

    elem_t tester = ptr_elem("notexisting");

    ioopm_hash_table_insert(h, k1 ,v1);
    ioopm_hash_table_insert(h, k2, v2);
    ioopm_hash_table_insert(h, k3, v3);

    CU_ASSERT_TRUE(str_eq(ioopm_hash_table_remove(h, tester), v4));
    CU_ASSERT_TRUE(str_eq(ioopm_hash_table_remove(h, k1),  v1));
    CU_ASSERT_TRUE(str_eq(ioopm_hash_table_remove(h, k2), v2));
    CU_ASSERT_TRUE(str_eq(ioopm_hash_table_remove(h, k3), v3));

    ioopm_hash_table_destroy(h);
}
void test_remove_single_entry()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, int_eq);
    elem_t v = ptr_elem("val");
    ioopm_hash_table_insert(h, int_elem(1), v);
    CU_ASSERT_TRUE(int_eq(ioopm_hash_table_remove(h, int_elem(1)), v));
    ioopm_hash_table_destroy(h);
}

void test_remove_null_entry()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, int_eq);
    CU_ASSERT_TRUE(int_eq(ioopm_hash_table_remove(h, int_elem(2)), void_elem(NULL)));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_size_is_one(void)
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(h), 0);
    elem_t v = ptr_elem("hello");
    ioopm_hash_table_insert(h, int_elem(0), v);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(h), 1);
    ioopm_hash_table_destroy(h);
}

void test_hash_table_size_is_empty(void)
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(h), 0);
    ioopm_hash_table_destroy(h);
}

void test_hash_table_is_empty()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_clear()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);

    elem_t v = ptr_elem("a");
    ioopm_hash_table_insert(h, int_elem(0), v);
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_clear(h);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_clear_empty()
{
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);
    ioopm_hash_table_clear(h);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(h));
    ioopm_hash_table_destroy(h);
}

void test_hash_table_find_keys()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, int_eq);
    int keys[6] = {-1, 100, 99, 0, 10, 42};
    bool found[6] = {false};
    elem_t v = int_elem(123);

    for (int i = 0; i < 6; i++)
    {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), v);
    }

    ioopm_list_t *ht_keys = ioopm_hash_table_keys(ht);
    // int ht_keys[6] = {-1, 100, 99, 0, 10, 42};

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (int_eq(ioopm_linked_list_get(ht_keys, i), int_elem(keys[j])))
            {
                found[j] = true;
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }

    ioopm_linked_list_destroy(ht_keys);
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_values()
{
    // const arr_size = 5;
    ioopm_hash_table_t *h = ioopm_hash_table_create(example_hash_function, str_eq);
    int keys[5] = {3, 4, 5, 6, 7};
    char *values[5] = {"three", "four", "five", "six", "seven"};
    bool found[5] = {false};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert(h, int_elem(keys[i]), ptr_elem(values[i]));
    }

    ioopm_list_t *hash_keys = ioopm_hash_table_keys(h);
    elem_t *hash_values = ioopm_hash_table_values(h);

    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < 5; k++)
        {
            // TODO: check that key corresponds to correct value.
            if (str_eq(ptr_elem(values[j]), hash_values[k]) == 0)
            {
                found[j] = true;
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }
    ioopm_linked_list_destroy(hash_keys);
    free(hash_values);
    //ioopm_destroy_hash_table_values(hash_values);
    ioopm_hash_table_destroy(h);
}

void test_hash_table_has_value(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, str_eq);
    int keys[5] = {3, 4, 5, 6, 7};
    char *values[5] = {"three", "four", "five", "six", "seven"};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }

    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("three")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("one")));
    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("six")));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, void_elem(NULL)));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_has_key(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, int_eq);
    int keys[5] = {3, 4, 5, 6, 7};
    char *values[5] = {"three", "four", "five", "six", "seven"};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(3)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(7)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(0)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(10)));
    // CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, NULL));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(8364)));

    ioopm_hash_table_destroy(ht);
}

// Predicates for hash_table_any/all.
bool all_keys_are_divisible_by_two(elem_t key, elem_t unused, void *x)
{
    return key.data.val % 2 == 0;
}

// Predicates for hash_table_any/all.
bool all_keys_are_lesser_than_nine(elem_t key, elem_t unused, void *x)
{
    return key.data.val < 9;
}

bool any_key_is_x(elem_t key, elem_t unused, void *x)
{
    if (x == NULL)
    {
        return false;
    }
    return key.data.val == *(int *)x;
}

// Predicates for hash_table_any/all.
bool any_key_is_greater_than_nine(elem_t key, elem_t unused, void *x)
{
    return key.data.val < 1;
}

// Predicates for hash_table_any/all.
bool any_key_is_divisible_by_two(elem_t key, elem_t unused, void *x)
{
    return key.data.val % 2 == 0;
}

/// @brief concatinates two strings and returns a new string.
/// @param str1
/// @param str2
/// @return new string = str1 + str2 that is allocated on the heap.
char *concat_string(char *str1, char *str2)
{
    int str_len = strlen(str1) + strlen(str2) + 1;
    char *new_str = calloc(str_len, sizeof(char *));
    strcpy(new_str, str1);
    strcat(new_str, str2);
    return new_str;
}

/// @brief Used in a testcase for hash_table_apply_to_all. It will concatinate value + string_add
/// @param key ??? not really used..
/// @param value ptr to value in hashtable.
/// @param string_add added to value
void apply_string_to_value(elem_t key, elem_t *value, void *string_add)
{
    char *new_val = concat_string(value->data.str, (char *) string_add);

    // If allocation fails.
    if (new_val == NULL)
    {
        printf("Error: could not allocate memory for new value in apply_string_to_value.\n");
        return;
    }

    value->data.str = new_val;

    free(new_val);
}

void test_hash_table_all(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, str_eq);
    int keys[5] =  {3, 4, 5, 6, 7};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem("hello"));
    }
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, all_keys_are_divisible_by_two, NULL));
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, all_keys_are_lesser_than_nine, NULL));
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_any(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, int_eq);

    int keys[5] = {3, 4, 5, 6, 7};
    int not_exist[5] = {8, 9, 10, 11, 12};

    for (int i = 0; i < 5; i++)
    {
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem("hello"));
    }

    CU_ASSERT_TRUE(ioopm_hash_table_any(ht, any_key_is_divisible_by_two, NULL));
    CU_ASSERT_FALSE(ioopm_hash_table_any(ht, any_key_is_greater_than_nine, NULL));

    for (int i = 0; i < 5; i++)
    {
        // these keys does exist so these should be equal (true).
        CU_ASSERT_EQUAL(ioopm_hash_table_any(ht, any_key_is_x, &keys[i]), ioopm_hash_table_has_key(ht, int_elem(keys[i])));

        // these keys does not exist in the table and should therefore be equal (false).
        CU_ASSERT_EQUAL(ioopm_hash_table_any(ht, any_key_is_x, &not_exist[i]), ioopm_hash_table_has_key(ht, int_elem(not_exist[i])))
    }
    // NULL handled correctly.
    // CU_ASSERT_EQUAL(ioopm_hash_table_any(ht, any_key_is_x, NULL), ioopm_hash_table_has_key(ht, NULL));

    ioopm_hash_table_destroy(ht);
}

void test_hash_table_apply_all(void) 
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(example_hash_function, int_eq);
    int keys[5] = {3, 4, 5, 6, 7};

    // Add five entries to hashtable, with value "hello".
    for (int i = 0; i < 5; i++)
    {
        elem_t v = ptr_elem("hello");
        ioopm_hash_table_insert(ht, int_elem(keys[i]), v);
    }

    // Check that previous entries were added correctly, and has value "hello".
    for (int i = 0; i < 5; i++)
    {
        CU_ASSERT_TRUE(str_eq(ioopm_hash_table_lookup(ht, int_elem(keys[i])).value, ptr_elem("hello")));
    }

    // Apply function
    ioopm_hash_table_apply_to_all(ht, apply_string_to_value, "world");

    // Check that all entries now contains "helloworld" as we expect.
    for (int i = 0; i < 5; i++)
    {
        elem_t entry_val = ioopm_hash_table_lookup(ht, int_elem(keys[i])).value;
        CU_ASSERT_TRUE(str_eq(entry_val, ptr_elem("helloworld")));
    }

    ioopm_hash_table_destroy(ht);
}

// run the tests

int main()
{

    // setup CUnit
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
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_has_values ]: both not existing and existing values work", test_hash_table_has_value) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_has_keys ]: both not existing and existing keys work", test_hash_table_has_key) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_all ]: Works for a false statement and a true statement", test_hash_table_all) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_any ]: Works for a false statement and a true statement", test_hash_table_any) == NULL) ||
        (CU_add_test(hashtable_test, "[ ioopm_hash_table_apply_all ]: applying function", test_hash_table_apply_all) == NULL) ||
        0)
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

  // Add outputfile and run tests automatically.
  CU_set_output_filename("test_results");
  CU_automated_run_tests();

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();

    return CU_get_error();
}