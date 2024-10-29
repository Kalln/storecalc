#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdbool.h>

#include "business_logic/database.h"
#include "business_logic/cart.h"
#include "business_logic/merch.h"


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

void test_create_database_successful(void) {
    database_t *db = create_database();
    CU_ASSERT_PTR_NOT_NULL(db);
    destroy_database(db);
}

void test_add_merch(void) {
    database_t *db = create_database();
    char *name = "A name";
    char *desc = "a description";
    int price = 12345;
    CU_ASSERT_TRUE(database_add_merch(db, name, desc, price));
    ioopm_hash_table_t *ht = database_get_items(db);
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, (elem_t) { .p = name}));
    destroy_database(db);
}

void test_create_destroy_cart_db(void) {
    cart_db_t *db = cart_db_create();
    CU_ASSERT_PTR_NOT_NULL(db);
    cart_db_destroy(db);
}

void test_new_cart_db_empty(void) {
    cart_db_t *db = cart_db_create();
    CU_ASSERT_EQUAL(cart_db_size(db), 0);
    cart_db_destroy(db);
}

void test_add_cart_increases_size(void) {
    cart_db_t *db = cart_db_create();
    cart_db_add_cart(db);
    CU_ASSERT_EQUAL(cart_db_size(db), 1);
    cart_db_add_cart(db);
    CU_ASSERT_EQUAL(cart_db_size(db), 2);
    cart_db_add_cart(db);
    CU_ASSERT_EQUAL(cart_db_size(db), 3);
    cart_db_add_cart(db);
    CU_ASSERT_EQUAL(cart_db_size(db), 4);
    cart_db_destroy(db);
}

void test_add_cart_ids_unique(void) {
    cart_db_t *db = cart_db_create();
    int id1 = cart_db_add_cart(db);
    int id2 = cart_db_add_cart(db);
    int id3 = cart_db_add_cart(db);
    int id4 = cart_db_add_cart(db);
    CU_ASSERT_NOT_EQUAL(id1, id2);
    CU_ASSERT_NOT_EQUAL(id1, id3);
    CU_ASSERT_NOT_EQUAL(id1, id4);
    CU_ASSERT_NOT_EQUAL(id2, id3);
    CU_ASSERT_NOT_EQUAL(id2, id4);
    CU_ASSERT_NOT_EQUAL(id3, id4);
    cart_db_destroy(db);
}

void test_removing_cart_decreases_size(void) {
    cart_db_t *db = cart_db_create();
    int id1 = cart_db_add_cart(db);
    int id2 = cart_db_add_cart(db);
    int id3 = cart_db_add_cart(db);
    int id4 = cart_db_add_cart(db);
    cart_db_remove_cart(db, id2);
    CU_ASSERT_EQUAL(cart_db_size(db), 3);
    cart_db_remove_cart(db, id3);
    CU_ASSERT_EQUAL(cart_db_size(db), 2);
    cart_db_remove_cart(db, id1);
    CU_ASSERT_EQUAL(cart_db_size(db), 1);
    cart_db_remove_cart(db, id4);
    CU_ASSERT_EQUAL(cart_db_size(db), 0);
    cart_db_destroy(db);
}

void test_calculate_cost(void) {
    database_t *db = create_database();
    char *name = "A name";
    char *desc = "a description";
    int price = 12345;
    database_add_merch(db, name, desc, price);
    merch_t *merch = get_merch_by_key(db, name);
    replenish_stock(db, merch, "Q42", 5);
    cart_t *cart = cart_create(1);

    cart_add_item(cart, 5, get_merch_by_key(db, "A name"));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 61725);
    database_add_cart(db);
    CU_ASSERT_EQUAL(database_calculate_cart_cost(db, 1), 0);
    cart_destroy(cart);
    destroy_database(db);
}

void test_remove_from_cart(void) {
    database_t *db = create_database();
    char *name = "A name";
    char *desc = "a description";
    int price = 12345;
    database_add_merch(db, name, desc, price);
    cart_t *cart = cart_create(1);
    merch_t *merch = get_merch_by_key(db, "A name");
    replenish_stock(db, merch, "Q42", 5);
    cart_add_item(cart, 5, merch);
    cart_remove_item(cart, 2, merch);
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 37035);
    cart_remove_item(cart, 2, merch);
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 12345);
    cart_remove_item(cart, 1, merch);
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 0);
    cart_destroy(cart);
    destroy_database(db);
}

void test_add_remove_multiple_times_from_cart(void) {
    database_t *db = create_database();
    char *name = "A name";
    char *desc = "a description";
    int price = 100;
    database_add_merch(db, name, desc, price);
    cart_t *cart = cart_create(1);
    merch_t *merch = get_merch_by_key(db, "A name");
    replenish_stock(db, merch, "Q42", 25);
    CU_ASSERT_TRUE(cart_add_item(cart, 5, merch));
    CU_ASSERT_TRUE(cart_add_item(cart, 2, merch));
    CU_ASSERT_TRUE(cart_add_item(cart, 3, merch));
    CU_ASSERT_TRUE(cart_add_item(cart, 4, merch));
    CU_ASSERT_TRUE(cart_add_item(cart, 6, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 2000);
    CU_ASSERT_TRUE(cart_remove_item(cart, 2, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 1800);
    CU_ASSERT_TRUE(cart_add_item(cart, 6, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 2400);
    CU_ASSERT_TRUE(cart_remove_item(cart, 8, merch));
    CU_ASSERT_TRUE(cart_remove_item(cart, 3, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 1300);
    CU_ASSERT_TRUE(cart_add_item(cart, 12, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 2500);
    CU_ASSERT_TRUE(cart_remove_item(cart, 2, merch));
    CU_ASSERT_TRUE(cart_remove_item(cart, 3, merch));
    CU_ASSERT_TRUE(cart_remove_item(cart, 15, merch));
    CU_ASSERT_TRUE(cart_remove_item(cart, 5, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 0);
    CU_ASSERT_TRUE(cart_add_item(cart, 5, merch));
    CU_ASSERT_EQUAL(cart_calculate_cost(cart), 500);
    cart_destroy(cart);
    destroy_database(db);
}

void test_remove_merch(void) {
    database_t *db = create_database();
    // ADD ITEM
    database_add_merch(db, "A name", "a description", 12345);
    database_add_merch(db, "NULL", "NULL", 0);

    // CHECK ITEM IS IN DATABASE
    CU_ASSERT_EQUAL(database_size(db), 2);
    merch_t *m = get_merch_by_key(db, "A name");
    CU_ASSERT_PTR_NOT_NULL(m);
    // REMOVE AND CHECK DB IS EMPTY
    CU_ASSERT_TRUE(database_remove_merch(db, m));

    // REMOVE NULL.
    CU_ASSERT_FALSE(database_remove_merch(db, NULL));
    CU_ASSERT_EQUAL(database_size(db), 1);

    destroy_database(db);
}

void test_edit_merch(void) {
    database_t *db = create_database();

    // ADD ITEM
    database_add_merch(db, "Product 1", "Very good product", 123452);
    database_add_merch(db, "Product 2", "Very good product 2", 21333);

    merch_t *item1 = get_merch_by_key(db, "Product 1");
    merch_t *item2 = get_merch_by_key(db, "Product 2");

    // EDIT ITEM.
    // edit description
    // merch_description.
    CU_ASSERT_TRUE(database_edit_description(item1, "Very bad product"));
    CU_ASSERT_EQUAL(strcmp(database_get_description(item1), "Very bad product"), 0);
    CU_ASSERT_EQUAL(strcmp(database_get_description(item2), "Very good product 2"), 0);

    // edit price
    CU_ASSERT_TRUE(database_edit_price(item1, 45299));
    CU_ASSERT_TRUE(database_edit_price(item2, 12377));
    CU_ASSERT_EQUAL(database_get_price(item1), 45299);
    CU_ASSERT_EQUAL(database_get_price(item2), 12377);
    CU_ASSERT_NOT_EQUAL(database_get_price(item1), database_get_price(item2));
    CU_ASSERT_NOT_EQUAL(database_get_price(item1), 123452);
    CU_ASSERT_NOT_EQUAL(database_get_price(item2), 21333);

    // edit title
    CU_ASSERT_TRUE(database_edit_title(db, item1, "New Product 1!"));
    item1 = get_merch_by_key(db, "New Product 1!");
    CU_ASSERT_PTR_NOT_NULL(item1);
    CU_ASSERT_EQUAL(strcmp(database_get_title(item1), "New Product 1!"), 0);

    // not possible to edit NULL
    merch_t *item3 = NULL;
    CU_ASSERT_FALSE(database_edit_title(db, item3, "New Product 1!"));
    CU_ASSERT_FALSE(database_edit_description(item3, "Very bad product"));
    CU_ASSERT_FALSE(merch_edit_description(item3, "Test description"));
    CU_ASSERT_FALSE(database_edit_price(item3, 45299));
    CU_ASSERT_FALSE(merch_edit_price(item3, 1234));

    destroy_database(db);
}

void test_multiple_shelf_locations(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This has multiple stokcs", 12344);
    merch_t *merch = get_merch_by_key(db, "Product 1");

    // test 1 shelf.
    ioopm_list_t *shelf_list = shelf_merch_list(merch);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(shelf_list));
    merch_add_stock_location(merch, "K33");

    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(shelf_list));
    shelf_t *res = (shelf_t *) ioopm_linked_list_get(shelf_list, 0).p;
    CU_ASSERT_EQUAL(strcmp(shelf_get_loc(res), "K33"), 0);
    CU_ASSERT_EQUAL(shelf_get_stock(res), 0);

    // test 2 shelf locations.

    merch_add_stock_location(merch, "A44");
    res = (shelf_t *) ioopm_linked_list_get(shelf_list, 1).p;
    CU_ASSERT_EQUAL(strcmp(shelf_get_loc(res), "A44"), 0);
    CU_ASSERT_EQUAL(shelf_get_stock(res), 0);
    destroy_database(db);
}

void test_add_stock(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");
    // get merch by key null


    CU_ASSERT_EQUAL(merch_get_stock(merch), 0);

    merch_add_stock_location(merch, "A10");
    CU_ASSERT_FALSE(shelf_add_stock(merch, "K33", 10));
    CU_ASSERT_EQUAL(merch_get_stock(merch), 0);

    CU_ASSERT_TRUE(shelf_add_stock(merch, "A10", 10));
    CU_ASSERT_EQUAL(merch_get_stock(merch), 10);
    shelf_t *a10 = ioopm_linked_list_get(shelf_merch_list(merch), 0).p;
    CU_ASSERT_EQUAL(shelf_get_stock(a10), 10);

    destroy_database(db);
}

void test_remove_stock(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");
    ioopm_hash_table_t *shelf_db = database_get_shelf_ht(db);
    merch_add_stock_location(merch, "K30");
    merch_add_stock_location(merch, "L40");
    CU_ASSERT_TRUE(shelf_add_stock(merch,"K30", 10));
    CU_ASSERT_TRUE(shelf_add_stock(merch, "L40", 2));

    // REMOVE FROM STOCK

    CU_ASSERT_TRUE(merch_remove_stock(shelf_db, merch, 3)); // 12-3 = 9 left in stock..7 left in K33.
    CU_ASSERT_EQUAL(merch_get_stock(merch), 9);
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 0).p), 7);
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 1).p), 2);

    CU_ASSERT_TRUE(merch_remove_stock(shelf_db, merch, 7)); // 9-7 = 2 left in stock..0 left in K33.. 2 i L40
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 0).p), 2);


    CU_ASSERT_FALSE(merch_remove_stock(shelf_db, merch, 3)); // 2 - 3 =-1 not possible, will be false. no changes made
    CU_ASSERT_EQUAL(merch_get_stock(merch), 2);
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 0).p), 2);

    merch_add_stock_location(merch, "K30");
    CU_ASSERT_TRUE(shelf_add_stock(merch, "K30", 5)); // new stock is 5 + 2 = 7
    CU_ASSERT_EQUAL(merch_get_stock(merch), 7);
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 0).p), 2);
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 1).p), 5);

    CU_ASSERT_TRUE(merch_remove_stock(shelf_db, merch, 6)); // 1 stock left, 0 in L40, 1 in K30.
    CU_ASSERT_EQUAL(merch_get_stock(merch), 1);
    CU_ASSERT_EQUAL(shelf_get_stock(ioopm_linked_list_get(shelf_merch_list(merch), 0).p), 1);

    destroy_database(db);
}

void test_reservecheckout_stock(void){
    database_t *db = create_database();
    database_add_merch(db, "PRODUCT 1", "Description of product 1", 12345);

    // ADD SHELF K33 + ADD 10 STOCK.
    merch_t *merch_ptr = get_merch_by_key(db, "PRODUCT 1");
    CU_ASSERT_TRUE(database_add_shelf(db, merch_ptr, "K33"));
    CU_ASSERT_TRUE(database_add_stock_to_shelf(db, merch_ptr, "K33", 10));

    // Add shelf A99 + Add 5 stock.
    CU_ASSERT_TRUE(database_add_shelf(db, merch_ptr, "A99"));
    CU_ASSERT_TRUE(database_add_stock_to_shelf(db, merch_ptr, "A99", 5));

    // Total stock is 15
    CU_ASSERT_EQUAL(merch_get_stock(merch_ptr), 15);
    CU_ASSERT_EQUAL(merch_get_reserved(merch_ptr), 0);
    CU_ASSERT_EQUAL(database_get_available_stock(merch_ptr), 15);

    // Reserved is 0.
    CU_ASSERT_EQUAL(database_get_available_stock(merch_ptr), 15);

    // Reserve 5
    CU_ASSERT_TRUE(database_reserve_item_stock(db, "PRODUCT 1", 5));
    CU_ASSERT_FALSE(database_reserve_item_stock(db, "PRODUCT 1", 20));
    CU_ASSERT_EQUAL(database_get_available_stock(merch_ptr), 10);


    // Checkout 5 > new total stock 10. 5 in K33 & 5 in A99

    CU_ASSERT_TRUE(database_checkout_item(db, "PRODUCT 1", 5));
    CU_ASSERT_EQUAL(merch_get_stock(merch_ptr), 10);
    CU_ASSERT_EQUAL(merch_get_reserved(merch_ptr), 0);

    ioopm_list_t *shelf_list = shelf_merch_list(merch_ptr);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(shelf_list), 2);
    shelf_t *k33 = (shelf_t *) ioopm_linked_list_get(shelf_list, 0).p;
    shelf_t *a99 = (shelf_t *) ioopm_linked_list_get(shelf_list, 1).p;
    CU_ASSERT_EQUAL(shelf_get_stock(k33), 5);
    CU_ASSERT_EQUAL(shelf_get_stock(a99), 5);

    // reserve 7
    CU_ASSERT_TRUE(database_reserve_item_stock(db, "PRODUCT 1", 7));
    CU_ASSERT_EQUAL(database_get_available_stock(merch_ptr), 3);

    // checkout 7 > new total stock 3. 0 in K33 (should not exist) 3 in A99.
    CU_ASSERT_TRUE(database_checkout_item(db, "PRODUCT 1", 7));
    //CU_ASSERT_EQUAL(ioopm_linked_list_size(shelf_list), 1);
    CU_ASSERT_EQUAL(merch_get_stock(merch_ptr), 3);
    CU_ASSERT_EQUAL(merch_get_reserved(merch_ptr), 0);
    CU_ASSERT_EQUAL(shelf_get_stock(a99), 3);

    destroy_database(db);
}

void test_reserve_unreserve(void){
    database_t *db = create_database();
    database_add_merch(db, "Merch", "A merch", 321);
    merch_t *merch = get_merch_by_key(db, "Merch");
    CU_ASSERT_EQUAL(merch_get_reserved(merch), 0);
    CU_ASSERT_TRUE(merch_reserve_stock(merch, 0));
    CU_ASSERT_EQUAL(merch_get_reserved(merch), 0);
    CU_ASSERT_FALSE(merch_reserve_stock(merch, 1));
    CU_ASSERT_FALSE(merch_reserve_stock(merch, 1000));
    CU_ASSERT_EQUAL(merch_get_reserved(merch), 0);

    replenish_stock(db, merch, "M42", 5);

    CU_ASSERT_EQUAL(merch_get_reserved(merch), 0);
    CU_ASSERT_TRUE(merch_reserve_stock(merch, 0));
    CU_ASSERT_EQUAL(merch_get_reserved(merch), 0);
    CU_ASSERT_TRUE(merch_reserve_stock(merch, 1));
    CU_ASSERT_EQUAL(merch_get_reserved(merch), 1);
    CU_ASSERT_FALSE(merch_reserve_stock(merch, 1000));
    CU_ASSERT_EQUAL(merch_get_reserved(merch), 1);
    CU_ASSERT_FALSE(merch_reserve_stock(merch, 5));
    CU_ASSERT_TRUE(merch_reserve_stock(merch, 4));
    CU_ASSERT_FALSE(merch_reserve_stock(merch, 1));

    replenish_stock(db, merch, "C89", 7);

    CU_ASSERT_FALSE(merch_reserve_stock(merch, 8));
    CU_ASSERT_TRUE(merch_reserve_stock(merch, 7));
    CU_ASSERT_TRUE(merch_unreserve(merch, 10));
    CU_ASSERT_TRUE(merch_unreserve(merch, 2));
    CU_ASSERT_TRUE(merch_unreserve(merch, 0));
    CU_ASSERT_FALSE(merch_unreserve(merch, 1));
    CU_ASSERT_FALSE(merch_reserve_stock(merch, 13));
    CU_ASSERT_TRUE(merch_reserve_stock(merch, 12));

    destroy_database(db);
}

void test_merch_key_exists(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    CU_ASSERT_PTR_NOT_NULL(get_merch_by_key(db, "Product 1"));
    CU_ASSERT_PTR_NULL(get_merch_by_key(db, "Product 2"));
    destroy_database(db);
}

void test_add_shelf_different_formats(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");
    // wrong format
    CU_ASSERT_FALSE(database_add_shelf(db, NULL, "A33"));
    CU_ASSERT_FALSE(database_add_shelf(db, merch, "b22"));
    CU_ASSERT_FALSE(database_add_shelf(db, merch, "A3a"));
    CU_ASSERT_TRUE(database_add_shelf(db, merch, "A33"));
    CU_ASSERT_FALSE(database_add_shelf(db, merch, "A33"));
    CU_ASSERT_FALSE(database_add_shelf(db, merch, "A"));
    CU_ASSERT_FALSE(database_add_shelf(db, merch, "A3"));

    // correct format
    CU_ASSERT_TRUE(database_add_shelf(db, merch, "Z11"));
    CU_ASSERT_FALSE(database_add_shelf(db, merch, "Z11"));
    CU_ASSERT_TRUE(database_add_shelf(db, merch, "Z99"));
    CU_ASSERT_TRUE(database_add_shelf(db, merch, "A11"));

    destroy_database(db);
}

void test_removed_shelf_does_not_exist_in_ht(void) {
    database_t *db = create_database();

    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");

    // Single shelf

    CU_ASSERT_TRUE(database_add_shelf(db, merch, "B10"));
    CU_ASSERT_TRUE(shelf_exists(db, "B10"));
    CU_ASSERT_TRUE(database_add_stock_to_shelf(db, merch, "B10", 5));

    CU_ASSERT_TRUE(database_reserve_item_stock(db, "Product 1", 5));
    CU_ASSERT_TRUE(database_checkout_item(db, "Product 1", 5));

    CU_ASSERT_FALSE(shelf_exists(db, "B10"));


    // multiple shelf

    CU_ASSERT_TRUE(database_add_shelf(db, merch, "B10"));
    CU_ASSERT_TRUE(database_add_shelf(db, merch, "C10"));

    CU_ASSERT_TRUE(database_add_stock_to_shelf(db, merch, "B10", 5));
    CU_ASSERT_TRUE(database_add_stock_to_shelf(db, merch, "C10", 5));

    CU_ASSERT_TRUE(shelf_exists(db, "B10"));
    CU_ASSERT_TRUE(shelf_exists(db, "C10"));

    CU_ASSERT_TRUE(database_reserve_item_stock(db, "Product 1", 5));
    CU_ASSERT_TRUE(database_checkout_item(db, "Product 1", 5));

    CU_ASSERT_FALSE(shelf_exists(db, "B10"));
    CU_ASSERT_TRUE(shelf_exists(db, "C10"));


    destroy_database(db);
}

void test_replenish_stock(void) {
    database_t *db = create_database();

    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");

    CU_ASSERT_TRUE(replenish_stock(db, merch, "A10", 10));
    CU_ASSERT_TRUE(replenish_stock(db, merch, "B99", 50));
    CU_ASSERT_TRUE(replenish_stock(db, merch, "A10", 10));

    CU_ASSERT_EQUAL(merch_get_stock(merch), 70);

    database_add_merch(db, "Product 2", "This is product 1", 12345);
    merch_t *merch2 = get_merch_by_key(db, "Product 2");

    CU_ASSERT_FALSE(replenish_stock(db, merch2, "A10", 10));
    CU_ASSERT_EQUAL(merch_get_stock(merch), 70);
    CU_ASSERT_EQUAL(merch_get_stock(merch2), 0);

    CU_ASSERT_TRUE(replenish_stock(db, merch2, "B10", 10));
    CU_ASSERT_TRUE(replenish_stock(db, merch2, "L10", 80));
    CU_ASSERT_EQUAL(merch_get_stock(merch2), 90);

    destroy_database(db);
}

void test_database_add_remove_cart(void) {
    database_t *db = create_database();
    for (int i = 1; i <= 20; i++)
    {
        CU_ASSERT_EQUAL(database_add_cart(db), i);
    }
    database_remove_cart(db, 20);
    for (int i = 1; i <= 10; i++)
    {
        database_remove_cart(db, i);
    }

    destroy_database(db);
}

void test_add_same_merch(void) {
    database_t *db = create_database();
    CU_ASSERT_TRUE(database_add_merch(db, "Product 1", "This is product 1", 12345));
    CU_ASSERT_FALSE(database_add_merch(db, "Product 1", "This is product 1", 12345));
    destroy_database(db);
}

void test_remove_item_while_in_cart(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");
    int id = database_add_cart(db);
    CU_ASSERT_TRUE(replenish_stock(db, merch, "Q42", 5));
    CU_ASSERT_TRUE(database_add_item_to_cart(db, id, "Product 1", 1));
    CU_ASSERT_FALSE(database_remove_merch(db, merch));
    destroy_database(db);
}

void test_add_many_items_to_cart(void) {
    database_t *db = create_database();
    int count = 26;
    char *names[] = {
        "Alfa", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot", "Golf",
        "Hotel", "India", "Juliett", "Kilo", "Lima", "Mike", "November",
        "Oscar", "Papa", "Quebec", "Romeo", "Sierra", "Tango",
        "Uniform", "Victor", "Whiskey", "Xray", "Yankee", "Zulu"
    };
    char *descriptions[] = {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
    };
    char *shelves[] = {
        "A12", "B23", "C34", "D45", "E56", "F67", "G78",
        "H89", "I90", "J01", "K13", "L24", "M35",
        "N46", "O57", "P68", "Q79", "R80", "S91", "T02",
        "U14", "V25", "W36", "X47", "Y58", "Z69"
    };
    int costs[count];
    int quantities[count];
    int total_cost = 0;
    for (int i = 0; i < count; ++i) {
        costs[i] = i*i;
        quantities[i] = i + 5;
        total_cost += costs[i] * quantities[i];
    }

    int id = database_add_cart(db);
    for (int i = 0; i < count; ++i) {
        costs[i] = i*i;
        database_add_merch(db, names[i], descriptions[i], costs[i]);
        merch_t *merch = get_merch_by_key(db, names[i]);
        CU_ASSERT_PTR_NOT_NULL(merch);
        CU_ASSERT_TRUE(replenish_stock(db, merch, shelves[i], quantities[i]));
        CU_ASSERT_TRUE(database_add_item_to_cart(db, id, names[i], quantities[i]));
    }
    CU_ASSERT_EQUAL(database_calculate_cart_cost(db, id), total_cost);

    for (int i = 0; i < count; ++i) {
        CU_ASSERT_TRUE(database_remove_item_from_cart(db, id, names[i], quantities[i]));
    }
    destroy_database(db);
}

/// @brief Get the nth digit of a number (counting from the right)
/// @param value The number to get a digit of
/// @param index The index, where 0 means the least significant digit
/// @return The nth digit
static int digit(int value, int index) {
    if (index == 0) {
        return value % 10;
    } else {
        return digit(value / 10, index - 1);
    }
}

void test_add_1000_items_to_cart(void) {
    database_t *db = create_database();
    int count = 1000;
    char names[count][4];
    char descriptions[count][4];
    char shelves[count][4];
    long long costs[count];
    long long quantities[count];
    long long total_cost = 0;

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < 3; ++j) {
            /// AAA, BAA, CAA ..HJJ, IJJ, JJJ
            names[i][j] = digit(i, j) + 'A';
        }
        names[i][3] = '\0';
        for (int j = 0; j < 3; ++j) {
            /// AAA, AAB, AAC ..JJH, JJI, JJJ
            descriptions[i][j] = digit(i, 2-j) + 'A';
        }
        descriptions[i][3] = '\0';

        // A00, A01, A02 .. J97, J98, J99
        shelves[i][0] = digit(i, 2) + 'A';
        shelves[i][1] = digit(i, 1) + '0';
        shelves[i][2] = digit(i, 0) + '0';
        shelves[i][3] = '\0';

        costs[i] = i*2;
        quantities[i] = i + 5;
        total_cost += costs[i] * quantities[i];
    }

    int id = database_add_cart(db);
    for (int i = 0; i < count; ++i) {
        CU_ASSERT_TRUE(database_add_merch(db, names[i], descriptions[i], costs[i]));
        merch_t *merch = get_merch_by_key(db, names[i]);
        CU_ASSERT_PTR_NOT_NULL(merch);
        CU_ASSERT_TRUE(replenish_stock(db, merch, shelves[i], quantities[i]));
        CU_ASSERT_TRUE(database_add_item_to_cart(db, id, names[i], quantities[i]));
    }
    long long cost = database_calculate_cart_cost(db, id);
    CU_ASSERT_EQUAL(cost, total_cost);

    for (int i = 0; i < count; ++i) {
        CU_ASSERT_TRUE(database_remove_item_from_cart(db, id, names[i], quantities[i]));
    }
    destroy_database(db);
}

void test_add_to_cart_insufficient_stock(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    int id = database_add_cart(db);
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Product 1", 1));
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Product 1", 5));
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Product 1", 999));
    destroy_database(db);
}

void test_add_invalid_merch_to_cart(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    int id = database_add_cart(db);
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Non-existing product", 0));
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Non-existing product", 1));
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Non-existing product", 5));
    CU_ASSERT_FALSE(database_add_item_to_cart(db, id, "Non-existing product", 999));
    destroy_database(db);
}

void test_remove_invalid_merch_from_cart(void) {
    database_t *db = create_database();
    database_add_merch(db, "Apple", "An apple", 50);
    merch_t *merch = get_merch_by_key(db, "Apple");
    int id = database_add_cart(db);
    CU_ASSERT_TRUE(replenish_stock(db, merch, "Q42", 5));
    CU_ASSERT_TRUE(database_add_item_to_cart(db, id, "Apple", 1));

    CU_ASSERT_FALSE(database_remove_item_from_cart(db, id, "Banana", 1));
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, id, "Orange", 1));
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, id, "apple", 1))
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, id, "Apple ", 1));
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, id, "A", 1));

    CU_ASSERT_TRUE(database_remove_item_from_cart(db, id, "Apple", 1));

    destroy_database(db);
}

void test_cart_id_valid(void) {
    database_t *db = create_database();
    CU_ASSERT_FALSE(database_cart_id_valid(db, 0));
    CU_ASSERT_FALSE(database_cart_id_valid(db, 1));
    int id = database_add_cart(db);
    CU_ASSERT_TRUE(database_cart_id_valid(db, id));
    CU_ASSERT_FALSE(database_cart_id_valid(db, id - 1));
    CU_ASSERT_FALSE(database_cart_id_valid(db, id + 1));
    int id2 = database_add_cart(db);
    CU_ASSERT_TRUE(database_cart_id_valid(db, id2));
    database_remove_cart(db, id);
    CU_ASSERT_FALSE(database_cart_id_valid(db, id));
    CU_ASSERT_TRUE(database_cart_id_valid(db, id2));
    database_remove_cart(db, id2);
    CU_ASSERT_FALSE(database_cart_id_valid(db, id2));
    destroy_database(db);
}

void test_cart_remove_invalid(void) {
    database_t *db = create_database();
    CU_ASSERT_FALSE(database_remove_cart(db, 0));
    CU_ASSERT_FALSE(database_remove_cart(db, 1));
    int id = database_add_cart(db);
    CU_ASSERT_FALSE(database_remove_cart(db, id - 1));
    CU_ASSERT_FALSE(database_remove_cart(db, id + 1));
    int id2 = database_add_cart(db);
    CU_ASSERT_TRUE(database_remove_cart(db, id2));
    CU_ASSERT_FALSE(database_remove_cart(db, id2));
    CU_ASSERT_TRUE(database_remove_cart(db, id));
    CU_ASSERT_FALSE(database_remove_cart(db, id));
    destroy_database(db);
}

void test_shelf_list_contains_correct_list(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    merch_t *merch = get_merch_by_key(db, "Product 1");
    CU_ASSERT_TRUE(replenish_stock(db, merch, "Q42", 5));
    ioopm_list_t *ls = database_shelf_list(merch);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(ls), 1);

    destroy_database(db);
}

void test_special_add_stock(void) {
    database_t *db = create_database();
    CU_ASSERT_TRUE(database_add_merch(db, "Product 1", "This is product 1", 12345));
    CU_ASSERT_FALSE(database_add_stock_to_shelf(db, NULL, "Q42", 5));
    CU_ASSERT_FALSE(database_add_stock_to_shelf(db, get_merch_by_key(db, "Product 1"), "Q42", -5));
    destroy_database(db);
}

void test_special_desc_price_case(void) {
    database_t *db = create_database();
    CU_ASSERT_TRUE(database_add_merch(db, "Product 1", "This is product 1", 12345));
    CU_ASSERT_FALSE(merch_edit_description(NULL, "This is product 1"));
    CU_ASSERT_FALSE(merch_edit_price(NULL, 12345));
    destroy_database(db);
}

void test_unreserve(void) {
    database_t *db = create_database();
    database_add_merch(db, "Product 1", "This is product 1", 12345);
    CU_ASSERT_FALSE(merch_unreserve(get_merch_by_key(db, "Product 1"), 5));
    destroy_database(db);
}

void test_db_cart_operation_item(void) {
    database_t *db = create_database();
    CU_ASSERT_EQUAL(database_add_cart(db), 1); // id 1

    CU_ASSERT_TRUE(database_add_merch(db, "Product 1", "This is product 1", 12345));
    merch_t *merch = get_merch_by_key(db, "Product 1");
    CU_ASSERT_TRUE(replenish_stock(db, merch, "A42", 5));

    CU_ASSERT_TRUE(database_add_item_to_cart(db, 1, "Product 1", 3));

    CU_ASSERT_FALSE(database_remove_item_from_cart(db, 1, "Product 1", 4)); // not enough quantity
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, 1, NULL, 2)); // NULL not possible
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, 1, "Not existing product", 2)); // not existing product
    CU_ASSERT_FALSE(database_remove_item_from_cart(db, 3, "Product 1", 2)); // not existing cart

    CU_ASSERT_TRUE(database_remove_item_from_cart(db, 1, "Product 1", 2));

    // checkout

    CU_ASSERT_FALSE(database_checkout_cart(db, 2)); // wrong id
    CU_ASSERT_TRUE(database_checkout_cart(db, 1));

    CU_ASSERT_EQUAL(database_get_available_stock(merch), 4);


    destroy_database(db);
}

void test_shelf_cmp(void) {
    elem_t s1 = { .p = "A01" };
    elem_t s2 = { .p = "B01" };
    elem_t s3 = { .p = "A00" };
    elem_t s4 = { .p = "A10" };
    elem_t s5 = { .p = "A11" };
    elem_t s6 = { .p = "A01" };
    elem_t s7 = { .p = strdup(s1.p) };
    CU_ASSERT_TRUE(shelf_cmp(s1, s1));
    CU_ASSERT_FALSE(shelf_cmp(s1, s2));
    CU_ASSERT_FALSE(shelf_cmp(s1, s3));
    CU_ASSERT_FALSE(shelf_cmp(s1, s4));
    CU_ASSERT_FALSE(shelf_cmp(s1, s5));
    CU_ASSERT_TRUE(shelf_cmp(s1, s6));
    CU_ASSERT_TRUE(shelf_cmp(s1, s7));
    free(s7.p);
}

int main(void) {
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("Merchandise administrative suite", init_suite, clean_suite);
    CU_pSuite my_cart_suite = CU_add_suite("Cart administrative suite", init_suite, clean_suite);
    if (my_test_suite == NULL || my_cart_suite == NULL) {
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
        (CU_add_test(my_test_suite, "create_database | Creating database", test_create_database_successful) == NULL)
        || (CU_add_test(my_cart_suite, "create/destroy cart database", test_create_destroy_cart_db) == NULL)
        || (CU_add_test(my_cart_suite, "new cart database is empty", test_new_cart_db_empty) == NULL)
        || (CU_add_test(my_cart_suite, "test adding carts increases size", test_add_cart_increases_size) == NULL)
        || (CU_add_test(my_cart_suite, "test adding carts generates unique ids", test_add_cart_ids_unique) == NULL)
        || (CU_add_test(my_cart_suite, "Removing cart decreases size", test_removing_cart_decreases_size) == NULL)
        || (CU_add_test(my_cart_suite, "Calculate cart cost", test_calculate_cost) == NULL)
        || (CU_add_test(my_cart_suite, "Removing merch from cart", test_remove_from_cart) == NULL)
        || (CU_add_test(my_cart_suite, "Adding and removing same merch multiple times", test_add_remove_multiple_times_from_cart) == NULL)
        || (CU_add_test(my_cart_suite, "database add remove cart", test_database_add_remove_cart) == NULL)
        || (CU_add_test(my_cart_suite, "Test cart id valid", test_cart_id_valid) == NULL)
        || (CU_add_test(my_cart_suite, "Test remove invalid id cart", test_cart_remove_invalid) == NULL)
        || (CU_add_test(my_cart_suite, "remove item not possible while in cart", test_remove_item_while_in_cart) == NULL)
        || (CU_add_test(my_cart_suite, "Add many items to cart", test_add_many_items_to_cart) == NULL)
        || (CU_add_test(my_cart_suite, "Add 1000 items to cart", test_add_1000_items_to_cart) == NULL)
        || (CU_add_test(my_cart_suite, "Add item to cart fails with insufficient stock", test_add_to_cart_insufficient_stock) == NULL)
        || (CU_add_test(my_cart_suite, "Add item with invalid name", test_add_invalid_merch_to_cart) == NULL)
        || (CU_add_test(my_cart_suite, "Remove item not in cart", test_remove_invalid_merch_from_cart) == NULL)
        || (CU_add_test(my_cart_suite, "database cart operations", test_db_cart_operation_item) == NULL)
        || (CU_add_test(my_test_suite, "adding merch", test_add_merch) == NULL)
        || (CU_add_test(my_test_suite, "Remove merch from database", test_remove_merch) == NULL)
        || (CU_add_test(my_test_suite, "Edit merch title/description/price", test_edit_merch) == NULL)
        || (CU_add_test(my_test_suite, "Merch have multiple storage locations", test_multiple_shelf_locations) == NULL)
        || (CU_add_test(my_test_suite, "add stock to shelf location + merch", test_add_stock) == NULL)
        || (CU_add_test(my_test_suite, "remove stock to shelf location + merch", test_remove_stock) == NULL)
        || (CU_add_test(my_test_suite, "reserve/checkout stock", test_reservecheckout_stock) == NULL)
        || (CU_add_test(my_test_suite, "reserve/unreserve stock", test_reserve_unreserve) == NULL)
        || (CU_add_test(my_test_suite, "get merch by key returns null when merch not exist", test_merch_key_exists) == NULL)
        || (CU_add_test(my_test_suite, "can have shelfs in different format", test_add_shelf_different_formats) == NULL)
        || (CU_add_test(my_test_suite, "shelf ht removes shelf when item stock runs out", test_removed_shelf_does_not_exist_in_ht) == NULL)
        || (CU_add_test(my_test_suite, "replenish function", test_replenish_stock) == NULL)
        || (CU_add_test(my_test_suite, "item with two same name not possible", test_add_same_merch) == NULL)
        || (CU_add_test(my_test_suite, "shelf list is correct", test_shelf_list_contains_correct_list) == NULL)
        || (CU_add_test(my_test_suite, "special cases add stock", test_special_add_stock) == NULL)
        || (CU_add_test(my_test_suite, "special cases desc, price", test_special_desc_price_case) == NULL)
        || (CU_add_test(my_test_suite, "unreserve more than is reserved not possible", test_unreserve) == NULL)
        || (CU_add_test(my_test_suite, "Test shelf comparison", test_shelf_cmp) == NULL)
    ) {
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
