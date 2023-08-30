#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int string_length(char *str) {

    int counter = 0;

    while (str[counter] != '\0') {
        counter++;
    }

    return counter;
}

void print(char *str) {
    int len = string_length(str);

    for (int i = 0; i <= len; i++) {
        putchar(str[i]);
    }
}

void println(char *str) {
    print(str);
    putchar('\n');
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s words or string", argv[0]);
  }
  else {
    for (int i = 1; i < argc; ++i) {
      int expected = strlen(argv[i]);
      int actual   = string_length(argv[i]);
      printf("strlen(\"%s\")=%d\t\tstring_length(\"%s\")=%d\n",
             argv[i], expected, argv[i], actual);
    }
  }
  printf("\n\n print: ");
  print(argv[1]);
  puts("test");
  println("test");
  return 0;
}