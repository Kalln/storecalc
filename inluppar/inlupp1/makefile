main: myprog

C_COMPILER     	= gcc
C_OPTIONS      	= -Wall -pedantic -g -lgcov
C_LINK_OPTIONS 	= -lm 
CUNIT_LINK     	= -lcunit
CCOV 			= -fprofile-arcs -ftest-coverage

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $(CCOV) $^ -c

myprog: hashtable_tests.o hashtable.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@

myprog.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.
	

# TESTS
test1: hashtable_tests.o hashtable.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

test: test1
	./test1

lt_test: linked_list_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

iter_test: iterator_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

memtest: test1
	valgrind --leak-check=full --error-exitcode=1 ./test1

iter_memtest: iter_test
	valgrind --leak-check=full --error-exitcode=1 ./iter_test

lt_memtest: lt_test
	valgrind --leak-check=full --error-exitcode=1 ./lt_test

hash_cov: hashtable_tests.o hashtable.o common.o linked_list.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

iterator_cov: iterator_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

linked_cov: linked_list_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)
	
ltcov: linked_cov iterator_cov
	./linked_cov
	./iterator_cov
	gcov linked_list.c

clean:
	rm -f *.o myprog test1 lt_test iter_test linked_cov iterator_cov *.gcda *.gcno *.gcov

.PHONY: test clean lt_test 
