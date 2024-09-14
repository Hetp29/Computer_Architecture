#include <stdio.h>
#include <stdlib.h>

int 
main(int argc, char **argv)
{
    // if arguments are passed, print them
    if (argc > 1){
        printf("Arguments passed to the program:\n");
        for (int i = 0; i < argc; i++) {
            printf("Argument %d: %s\n", i, argv[i]); //this prints each argument to the next line
        }
    } else {
        printf("No arguments passed to the program.\n");
    }

    return 0;
}

//for loop has (initialization; condition; increment)
