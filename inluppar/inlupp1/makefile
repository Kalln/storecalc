main: myprog

C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

myprog: hashtable_tests.o hashtable.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@

myprog.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.
	

# TESTS
test1: hashtable_tests.o hashtable.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK)

test: test1
	./test1

memtest: test1
	valgrind --leak-check=full --error-exitcode=1 ./test1



clean:
	rm -f *.o myprog
