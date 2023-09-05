/* 
* UTILS VERSION 3.0
* utils.c and utils.h, is for lab 3.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"



bool not_empty(char *str) {
    return strlen(str) > 0;
}



/*
* clear_input_buffer removes stdin buffert from input that is 'waste'.
*/
void clear_input_buffer() {
    int c;
    do {
        c = getchar();
    }
    while(c != '\n' && c != EOF);
}

bool is_digit(char c) {
    if ('0' <= c && c <= '9') {
        return true;
    } 
    else {
        return false;
    }
}

bool is_number(char *str) {
    if (!not_empty(str)) { return false; }
    // ta fram längden av str
    int len = strlen(str);
    // kontrollera negativa tal
    if (str[0] == '-' && str[1] == '\0') {
        return false;
    }
    // loopa genom str, kolla om varje 
    // tecken är en siffra, om nej -> inte int
    for (int i = str[0] == '-'; i < len; i++) {
        if (!is_digit(str[i])) {
            return false;
        }
    }
    return true;
}

void print(char *str) {
    int len = strlen(str);

    for (int i = 0; i <= len; i++) {
        putchar(str[i]);
    }
}

void println(char *str) {
    print(str);
    putchar('\n');
}

/*
* reads a string from user keyboard.
* guarantees that the buf is <= buf_size -> it will cut of the input.
*
* buf {*char}       - pointer to the buffer were the string is added.
* buf_size {int}    - the size of buf.
* returns {int}     - amount of char in user input. 
*/
int read_string(char *buf, int buf_size) {
    int MAX_BUF_SIZE = buf_size - 1;
    int counter = 0;
    int conversion = 0;
    char c;

    do {
        conversion = scanf("%c", &c);

        if (counter == MAX_BUF_SIZE) {
            // If we have reached the buf_size, we will terminate the string,
            // to prevent buffer overflow. This means that the user probably
            // has entered to many characters and we will need to empty the stdin
            // buffer to not make it a entry next time we run scanf.
            buf[counter] = '\0';
            clear_input_buffer();
            break;
        }
        else if (c == '\n' || c == EOF) {
            // If newline or EOF, we are done, so then we terminate the string.
            // and break the loop.
            buf[counter] = '\0';
            break;
        }
        else {
            // If no errors or cases appear, we add the char to the buf array,
            // and increase the counter. This is the only case we counter++
            // since other cases will break the loop.
            buf[counter] = c;
            counter++;
        }

    }
    while (conversion == 1);

    return counter;
}

answer_t ask_question(char *question, check_func check, convert_func convert) {
    int buf_siz = 255;
    char buf[buf_siz];


    do {
        printf("%s\n", question);
        read_string(buf, buf_siz);

        if (check(buf)) {
            break;
        }
    }
    while(true);

    answer_t result = convert(buf);
    return result;

}

int ask_question_int(char *question) {
    return ask_question(question, is_number, (convert_func) atoi).int_value;
}


/*
* Asks user for a question and returns the buf.
* It will keep on asking if the user does not enter anything.
*/
char *ask_question_string(char *question) {
    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}




