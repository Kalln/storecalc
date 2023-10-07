# IOOPM Hashtable
IOOPM hashtable consists of three C files. 
* hashtable.c
* linked_list.c
* common.c
  
Each of these files have been provided with a header file with full documentation of how each public function works.

This hashtable module does also provide test files as well as larger files that can test out the module. On how to run each test, please check the "how to run" section of this readme.

## How to run
* "make" - will run the program "freq_count.c" and use the hashtable to keep track of how many times a word has appeared in the book: "Den svenska Arbetareskyddslagstiftningen och Yrkesinspektionen" by Georg Stjernstedt (1902).
* make test - Will run all unit tests for all files.
* make cov - Presents coverage for all files.

## Deisgn choices
The hashtable does not take responsibility for any values that require allocation. It is up to the user to free all values or keys that requires allocation. With that said the hashtable will not take ownership of the values or keys. The hashtable does only have ownership of itself, its bucket (linked list) and its entries.

# Initial profiling result

* For each input, what are the top 3 functions?

    **256k**

        str_eq , find_previous_entry_for_key , ioopm_hash_table_has_key

    **16k** 

        str_eq bucket_calc string_sum_hash

    **10k** 

        str_eq bucket_calc string_sum_hash

    **1k**

        str_eq bucket_calc string_sum_hash

    **small**

        str_eq, bucket_calc, string_sum_hash

* For each input, what are the top 3 functions in your code (that you have written), or is it library functions?
    
    256k

    16k

    10k

    1k

    small

* Are the top 3 functions in your code consistent across the inputs? Why? Why not?

    It's fairly consistent. **str_eq** as well as **bucket_calc** and **string_sum_hash** are the functions that are taking
    the most time in our program except for the biggest file. In the biggest file we have instead functions
    that look for the previous entry of an entry and also the function that checks if an key has already
    been inserted or not. The reason for this is probably beacuse the buckets gets too "full". Since we 
    only have 17 buckets these gets filled pretty quickly no matter what file. This means that we are more likely
    to traverse the linked list in each bucket to find our key. 

    If we have 256 000 words that we want to insert into the hashtable that would mean, in our current hashtable,
    that there would be around 15 000 entries in each bucket. Compare this to inserting 16 000 words (about 900
    entries into each bucket). This means that we don't have as long way to go into the end of the list as if we
    had 15 000 entries in each bucket.
     
* Is there some kind of trend? (Possibly several.)
  
  The trends seem very clear from our viewpoint. Comparing the values (str_eq) is the function that is used the most.
  This has to do with that we always have to compare entries and their values to either know if they already exist
  or if the don't exist. When we get a large amount of entries, it seems very clear that we have to traverse a 
  very long way into the linked list to check all the entries. This is very clear on 256k input were
  find_previous_entry_for_key as well as ioopm_hash_table_has_key is topping the amount of calls each function get.

* Do the results correspond with your expectations?

  We are not sure what we expected but we expected definetly that looking up a key and looking for the previous key 
  would take the most time. We expected this because of in cases of large entries it will take a long time traversing
  and checking the whole linked list to be sure that the key either exists or does note exist. We expected this to be
  the case especially with the larger text file.

* Based on these results, do you see a way to make your program go faster?


  Increasing the amount of buckets would probably result in a faster program. The more buckets we have, the more we can
  "split" up our entries in each bucket and the fewer entries we have in each bucket would result in a faster run time.
  This is because calculating the bucket takes O(1) time, while traversing a linked list could take O(n) time in worst
  case scenario. In the most optimal hashtable we would only have one entry per bucket. This would result that we can
  lookup a key in O(1) time. 

### 256K 

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 47.62      0.10     0.10 60633435     0.00     0.00  str_eq
 23.81      0.15     0.05   543743     0.00     0.00  find_previous_entry_for_key
  9.52      0.17     0.02   271872     0.00     0.00  ioopm_hash_table_has_key
  4.76      0.18     0.01   815615     0.00     0.00  string_sum_hash
  4.76      0.19     0.01   271872     0.00     0.00  process_word
  4.76      0.20     0.01   271871     0.00     0.00  ioopm_hash_table_lookup
  4.76      0.21     0.01                             _init
  0.00      0.21     0.00   815615     0.00     0.00  bucket_calc
  0.00      0.21     0.00   271872     0.00     0.00  ioopm_hash_table_insert
  0.00      0.21     0.00     4387     0.00     0.00  entry_create
  0.00      0.21     0.00     4387     0.00     0.00  entry_destroy
  0.00      0.21     0.00     4371     0.00     0.00  element_create
  0.00      0.21     0.00     4371     0.00     0.00  element_destroy
  0.00      0.21     0.00     4370     0.00     0.00  free_all_elements
  0.00      0.21     0.00     4370     0.00     0.00  ioopm_linked_list_append
  0.00      0.21     0.00     4368     0.00     0.00  ioopm_iterator_next
  0.00      0.21     0.00       17     0.00     0.00  bucket_destroy
  0.00      0.21     0.00        1     0.00     0.00  ioopm_hash_table_apply_to_all
  0.00      0.21     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.21     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.21     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.21     0.00        1     0.00     0.00  ioopm_hash_table_size
  0.00      0.21     0.00        1     0.00     0.00  ioopm_iterator_current
  0.00      0.21     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.21     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.21     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.21     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.21     0.00        1     0.00   198.85  process_file
  0.00      0.21     0.00        1     0.00     0.00  sort_keys
 

 ### SMALL

   %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00      125     0.00     0.00  str_eq
  0.00      0.00     0.00       80     0.00     0.00  bucket_calc
  0.00      0.00     0.00       80     0.00     0.00  string_sum_hash
  0.00      0.00     0.00       53     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00       44     0.00     0.00  entry_create
  0.00      0.00     0.00       44     0.00     0.00  entry_destroy
  0.00      0.00     0.00       28     0.00     0.00  element_create
  0.00      0.00     0.00       28     0.00     0.00  element_destroy
  0.00      0.00     0.00       27     0.00     0.00  free_all_elements
  0.00      0.00     0.00       27     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00       27     0.00     0.00  ioopm_hash_table_insert
  0.00      0.00     0.00       27     0.00     0.00  ioopm_linked_list_append
  0.00      0.00     0.00       27     0.00     0.00  process_word
  0.00      0.00     0.00       26     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.00     0.00       25     0.00     0.00  ioopm_iterator_next
  0.00      0.00     0.00       17     0.00     0.00  bucket_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_apply_to_all
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_size
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_current
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.00     0.00        1     0.00     0.00  process_file
  0.00      0.00     0.00        1     0.00     0.00  sort_keys


### 1K 

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00     6500     0.00     0.00  str_eq
  0.00      0.00     0.00     2999     0.00     0.00  bucket_calc
  0.00      0.00     0.00     2999     0.00     0.00  string_sum_hash
  0.00      0.00     0.00     1999     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00     1000     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00     1000     0.00     0.00  ioopm_hash_table_insert
  0.00      0.00     0.00     1000     0.00     0.00  process_word
  0.00      0.00     0.00      999     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.00     0.00       40     0.00     0.00  entry_create
  0.00      0.00     0.00       40     0.00     0.00  entry_destroy
  0.00      0.00     0.00       24     0.00     0.00  element_create
  0.00      0.00     0.00       24     0.00     0.00  element_destroy
  0.00      0.00     0.00       23     0.00     0.00  free_all_elements
  0.00      0.00     0.00       23     0.00     0.00  ioopm_linked_list_append
  0.00      0.00     0.00       21     0.00     0.00  ioopm_iterator_next
  0.00      0.00     0.00       17     0.00     0.00  bucket_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_apply_to_all
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_size
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_current
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.00     0.00        1     0.00     0.00  process_file
  0.00      0.00     0.00        1     0.00     0.00  sort_keys





 ### 10k 

    %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00   200524     0.00     0.00  str_eq
  0.00      0.00     0.00    29999     0.00     0.00  bucket_calc
  0.00      0.00     0.00    29999     0.00     0.00  string_sum_hash
  0.00      0.00     0.00    19999     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00    10000     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00    10000     0.00     0.00  ioopm_hash_table_insert
  0.00      0.00     0.00    10000     0.00     0.00  process_word
  0.00      0.00     0.00     9999     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.00     0.00      216     0.00     0.00  entry_create
  0.00      0.00     0.00      216     0.00     0.00  entry_destroy
  0.00      0.00     0.00      200     0.00     0.00  element_create
  0.00      0.00     0.00      200     0.00     0.00  element_destroy
  0.00      0.00     0.00      199     0.00     0.00  free_all_elements
  0.00      0.00     0.00      199     0.00     0.00  ioopm_linked_list_append
  0.00      0.00     0.00      197     0.00     0.00  ioopm_iterator_next
  0.00      0.00     0.00       17     0.00     0.00  bucket_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_apply_to_all
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.00     0.00        1     0.00     0.00  ioopm_hash_table_size
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_current
  0.00      0.00     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.00     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.00     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.00     0.00        1     0.00     0.00  process_file
  0.00      0.00     0.00        1     0.00     0.00  sort_keys

### 16k


  Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
100.00      0.01     0.01  3777060     0.00     0.00  str_eq
  0.00      0.01     0.00    50975     0.00     0.00  bucket_calc
  0.00      0.01     0.00    50975     0.00     0.00  string_sum_hash
  0.00      0.01     0.00    33983     0.00     0.00  find_previous_entry_for_key
  0.00      0.01     0.00    16992     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.01     0.00    16992     0.00     0.00  ioopm_hash_table_insert
  0.00      0.01     0.00    16992     0.00     0.00  process_word
  0.00      0.01     0.00    16991     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.01     0.00     4387     0.00     0.00  entry_create
  0.00      0.01     0.00     4387     0.00     0.00  entry_destroy
  0.00      0.01     0.00     4371     0.00     0.00  element_create
  0.00      0.01     0.00     4371     0.00     0.00  element_destroy
  0.00      0.01     0.00     4370     0.00     0.00  free_all_elements
  0.00      0.01     0.00     4370     0.00     0.00  ioopm_linked_list_append
  0.00      0.01     0.00     4368     0.00     0.00  ioopm_iterator_next
  0.00      0.01     0.00       17     0.00     0.00  bucket_destroy
  0.00      0.01     0.00        1     0.00     0.00  ioopm_hash_table_apply_to_all
  0.00      0.01     0.00        1     0.00     0.00  ioopm_hash_table_create
  0.00      0.01     0.00        1     0.00     0.00  ioopm_hash_table_destroy
  0.00      0.01     0.00        1     0.00     0.00  ioopm_hash_table_keys
  0.00      0.01     0.00        1     0.00     0.00  ioopm_hash_table_size
  0.00      0.01     0.00        1     0.00     0.00  ioopm_iterator_current
  0.00      0.01     0.00        1     0.00     0.00  ioopm_iterator_destroy
  0.00      0.01     0.00        1     0.00     0.00  ioopm_linked_list_create
  0.00      0.01     0.00        1     0.00     0.00  ioopm_linked_list_destroy
  0.00      0.01     0.00        1     0.00     0.00  ioopm_list_iterator
  0.00      0.01     0.00        1     0.00     9.14  process_file
  0.00      0.01     0.00        1     0.00     0.00  sort_keys