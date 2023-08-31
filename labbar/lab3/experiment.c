#include <stdio.h>
#include "utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*  typedef union {
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);


extern char *strdup(const char *);

bool not_empty(char *str) {
    return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func check, convert_func convert) {
    int buf_siz = 255;
    char buf[buf_siz];
    int size = 0;

    do {
        printf("%s\n", question);
        size = read_string(buf, buf_siz);

        if (check(buf)) {
            break;
        }
    }
    while(not_empty(buf));


    answer_t result = convert(buf);
    return result;

}


int ask_question_int(char *question) {
    answer_t answer = ask_question(question, is_number, (convert_func) atoi);
    return answer.int_value;
} */

int main(void) {
    char *val = 0;
    int val2 = 0;
    val = ask_question_string("Enter a name kingen: ");

    printf("val = %s\n", val);

    val2 = ask_question_int("Enter a number");
    printf("val2 = %d\n", val2);


    return 0;
}