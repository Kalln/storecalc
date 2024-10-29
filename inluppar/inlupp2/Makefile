.PHONY: clean coverage test test_leaks main test_frontend
.SUFFIXES:
all: main unit_tests
CC       = gcc
CFLAGS   = -Wall -pedantic -g -Werror
INCLUDES = -I /opt/homebrew/Cellar/cunit/2.1-3/include
LDLIBS   = -lm -lcunit
LDFLAGS  = -L /opt/homebrew/lib/

ifdef COVERAGE
CFLAGS += -coverage
LDFLAGS += -coverage
endif

ifdef SANITIZE
CFLAGS += -fsanitize=$(SANITIZE)
LDFLAGS += -fsanitize=$(SANITIZE)
endif

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
# On macOS, use leaks instead of valgrind
LEAK_SAN = leaks --atExit --
else
# Otherwise, use valgrind
LEAK_SAN = valgrind
endif

%.o:  %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

unit_tests.o: unit_tests.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

datastructures/%.o: datastructures/%.c
	$(MAKE) -C datastructures SANITIZE="$(SANITIZE)"

business_logic/backend.o: business_logic/merch.o business_logic/database.o business_logic/cart.o
	ld -r $^ -o $@

main: main.o user_interface/frontend.o utils/utils.o business_logic/backend.o datastructures/linked_list.o datastructures/hash_table.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

run: main
	./main

unit_tests: unit_tests.o business_logic/backend.o datastructures/linked_list.o datastructures/hash_table.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test: unit_tests
	./unit_tests

memtest: unit_tests
	$(LEAK_SAN) ./unit_tests

test_frontend: main
	./test_frontend.py

clean:
	$(MAKE) -C datastructures clean
	find . \( -type f -name "*.o" -o -name "*.gcno" -o -name "*.gcda" -o -name "*.info" \) -delete
	rm -f unit_tests main

docs:
	doxygen

coverage: clean
	$(MAKE) unit_tests COVERAGE=true
	./unit_tests
	lcov --capture --directory business_logic --output-file coverage.info --rc branch_coverage=1
	genhtml coverage.info --output-directory coverage --branch-coverage
