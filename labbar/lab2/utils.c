#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// hur fungerar den här?
void clear_input_buffer() {
    int c;
    do {
        // hur vet den var i minnet den ska hämta symbolen?
        // är tangentsbords buffret (stdin), vid en allokerad plats
        // vid körning av programmet?
        c = getchar();
    }
    while(c != '\n' && c != EOF);
}
int ask_question_int(char *question) {
    int result = 0;
    int conversion = 0; 
    do {
        printf("%s\n", question);

        // conversion blir 1 om det finns innehåll som är en int?
        conversion = scanf("%d", &result);
        
        clear_input_buffer();
        putchar('\n');
    }
    while (conversion < 1);
    return result;
}

/*
* reads a string from user keyboard.
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



char *ask_question_string(char *question) {
    char result[255];

    do {
        printf("%s\n", question);
        scanf("%s", result);
    }
    while (strlen(result) == 0);

    return strdup(result);
}


int main(void) {
    int buf_siz = 255;
    int read = 0;
    char buf[buf_siz];

    puts("Läs in en sträng:");
    read = read_string(buf, buf_siz);
    printf("'%s' (%d tecken)\n", buf, read);

    puts("Läs in en sträng till:");
    read = read_string(buf, buf_siz);
    printf("'%s' (%d tecken)\n", buf, read); 


    return 0;
}