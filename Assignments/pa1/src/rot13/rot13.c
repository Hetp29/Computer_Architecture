#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isalpha()

void rot13(const char *str) {
    while(*str) {
        char ch = *str; //current character
        if(isalpha(ch)) {
            if(ch >= 'a' && ch <= 'z') {
                ch = (ch - 'a' + 13) % 26 + 'a';
            }
            else if (ch >= 'A' && ch <= 'Z') {
                ch = (ch - 'A' + 13) % 26 + 'A';
            }
        }
        putchar(ch);
        str++;
    }
    putchar('\n');
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_SUCCESS;
    }

    rot13(argv[1]);

    return EXIT_SUCCESS;
}