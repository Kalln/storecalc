#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

// typedef
typedef union {
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);

extern char *strdup(const char *);

// check functions
bool is_number(char *str);
bool is_digit(char c);
bool not_empty(char *str);

// ask_question functions
int ask_question_int(char *question);
char *ask_question_string(char *question);
answer_t ask_question(char *question, check_func check, convert_func convert);

void print(char *str);
#endif