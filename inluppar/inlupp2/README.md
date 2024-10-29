## Dependencies
This assignment uses the hash table and linked list from inlupp1, written by Mauritz S. and Elias S. and utils.c written by Karl Widborg Kortelainen

## Unit tests
All commands should be run within the inlupp2/ directory
Build and run the unit tests
```
make test
```

Run unit tests and check for memory leaks (Uses Valgrind on linux etc. and `leaks` on macOS)
```
make memtest
```

Check the test coverage and produce a coverage report using gcov and lcov
```
make coverage
```
Current test coverage:
| File       | Line coverage | Branch coverage | Function coverage |
|------------|---------------|-----------------|-------------------|
| cart.c     | 99.1%         | 86.4%           | 100%              |
| database.c | 96.8%         | 85.1%           | 97.2%             |
| merch.c    | 99.2%         | 96.4%           | 100%              |

## Using the program
To run the database program, run the following command:
```
make run
```
This will build all the required files and start the interactive database program.
On startup, the program displays a menu with different options, which can
be selected by entering the highlighted character and then pressing enter.
