#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isalpha() and islower() to check is character is in alphabet and is lower case

void rot13(char *str) { 
//char str is a single character, char *str is pointer to first character in string
    while (*str) { 
        if (isalpha(*str)) { 
            if(islower(*str)) { 
                printf("%c", (*str - 'a' + 13) % 26 + 'a'); 
            }
            else {
                printf("%c", (*str - 'A' + 13) % 26 + 'A'); 
            }
        }
        else {
            printf("%c", *str); 
        }
        str++; 
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    rot13(argv[1]);

    return 0;
}