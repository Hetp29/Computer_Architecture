#include <stdio.h>
#include <ctype.h> //isalpha() and islower() to check is character is in alphabet and is lower case

void rot13(char *str) { //char * points to first character in array of characters (string)
//char str is a single character, char *str is pointer to first character in string
    while (*str) { //loop through entire string (runs till \0)
        if (isalpha(*str)) { //is character alphabetic
            if(islower(*str)) { //is character lowercase
                printf("%c", (*str - 'a' + 13) % 26 + 'a'); //lower case, shift 13 positions and wrap around
            }
            else {
                printf("%c", (*str - 'A' + 13) % 26 + 'A'); //shifts upper case letters
            }
        }
        else {
            printf("%c", *str); //non alphabetic characters
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