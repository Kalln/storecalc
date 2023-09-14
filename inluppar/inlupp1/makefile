run-hash:
	gcc -pedantic -g -Wall hashtable_tests.c hashtable.c -lcunit -o hashtable_tests
	./hashtable_tests

memtest: 
	gcc -g -Wall hashtable_tests.c hashtable.c -lcunit -o hashtable_tests
	valgrind --leak-check=full ./hashtable_tests

memtestoptimized: 
	gcc -g -Wall -O2 hashtable_tests.c hashtable.c -lcunit -o hashtable_tests
	valgrind --leak-check=full ./hashtable_tests
