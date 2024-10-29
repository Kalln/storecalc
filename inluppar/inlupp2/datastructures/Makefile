.PHONY: clean coverage
main: freq_count

C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g -Werror
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit -L /opt/homebrew/lib/
CUNIT_INCLUDE  = -I /opt/homebrew/Cellar/cunit/2.1-3/include

ifdef COVERAGE
C_OPTIONS += -coverage
C_LINK_OPTIONS += -coverage
endif

ifdef SANITIZE
C_OPTIONS += -fsanitize=$(SANITIZE)
C_LINK_OPTIONS += -fsanitize=$(SANITIZE)
endif


%.o:  %.c
	$(C_COMPILER) $(CUNIT_INCLUDE) $(C_OPTIONS) $^ -c

unit_tests: hash_table.o linked_list.o unit_tests.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ $(CUNIT_LINK) -o unit_tests

freq_count: hash_table.o linked_list.o freq_count.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ $(CUNIT_LINK) -o freq_count

clean:
	rm -f linked_list hash_table unit_tests
	rm -f ./*.o
	rm -f ./*.gcno
	rm -f ./*.gcda
	rm -f ./*.info

coverage: clean
	make unit_tests COVERAGE=true
	./unit_tests
	lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
	genhtml coverage.info --output-directory coverage --branch-coverage