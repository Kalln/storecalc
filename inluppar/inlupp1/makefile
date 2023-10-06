main: myprog

C_COMPILER     	= gcc
C_OPTIONS      	= -Wall -pedantic -g -lgcov
C_LINK_OPTIONS 	= -lm 
CUNIT_LINK     	= -lcunit
CCOV 			= -fprofile-arcs -ftest-coverage

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

myprog: hashtable_tests.o hashtable.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@

myprog.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.
	

# TESTS
test1: hashtable_tests.o hashtable.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

hash_test: hashtable_tests.o hashtable.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)
	./hash_test
	make clean

test: test1
	./test1

lt_test: linked_list_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

iter_test: iterator_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

# Memory tests
memtest: test1
	valgrind --leak-check=full --error-exitcode=1 ./test1

iter_memtest: iter_test
	valgrind --leak-check=full --error-exitcode=1 ./iter_test

lt_memtest: lt_test
	valgrind --leak-check=full --error-exitcode=1 ./lt_test

# Code coverage

hash_cov: hashtable_tests.o hashtable.o common.o linked_list.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

iterator_cov: iterator_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)

linked_cov: linked_list_tests.o linked_list.o common.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(CCOV) $^ -o $@ $(CUNIT_LINK)
	
ht_cov: hashtable.c linked_list.c common.c hashtable_tests.c linked_list.h hashtable.h common.h
	$(C_COMPILER) $(C_LINK_OPTIONS) $(word 1, $^) $(word 2, $^) $(word 3, $^) $(word 4, $^) -lgcov --coverage -o $@ $(CUNIT_LINK)

ltcov: linked_cov iterator_cov
	./linked_cov
	./iterator_cov
	gcov linked_list.c 

htcov: hash_cov
	./hash_cov
	gcov hashtable.c

# Freq count
freq_test: common.o linked_list.o hashtable.o freq_count.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@

prof: common.c linked_list.c hashtable.c freq_count.c
	gcc -o prof_req $^ -pg
	./prof_req freq_count/256k-words.txt
	gprof prof_req gmon.out > profile.txt


coverage:
	make ht_cov
	./ht_cov
	gcov ht_cov-hashtable
	lcov --capture --directory . --output-file coverage_ht.info
	genhtml coverage_ht.info --output-directory Cover


clean:
	rm -f *.o myprog test1 lt_test linked_list_tests linked_list_test hash_test

# remove coverage files
	rm -f ht_cov *.gcda *.gcno *.gcov *.html *.png *.css *.infogcov ht_cov-hashtable iter_test linked_cov  hash_cov coverage_ht.info iterator_cov
	rm -f freq_test prof_req gmon.out profile.txt

.PHONY: test clean lt_test ht_cov hash_test
