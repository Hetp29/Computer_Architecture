#include <stdio.h>
#include <ctype.h> //isalpha()

void rot13(const char *str) {
    char ch;
    while ((ch = *str++)) {
        if (isalpha(ch)) {
            if(islower(ch)) {
                putchar(((ch - 'a' + 13) % 26) + 'a');
            } else {
                putchar(((ch - 'A' + 13) % 26) + 'A');
            }
        } else {
            putchar(ch);
        }
    }
    putchar('\n');
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    rot13(argv[1]);

    return 0;
}