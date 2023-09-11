run_hash:
	gcc -g -Wall hashtable_tests.c hashtable.c -lcunit -o hashtable_tests
	./hashtable_tests

memtest: 
	gcc -g -Wall hashtable_tests.c hashtable.c -lcunit -o hashtable_tests
	valgrind ./hashtable_tests
