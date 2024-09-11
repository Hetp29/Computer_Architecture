#include <stdio.h> /*Header file with functions for input and output, such as printf*/
#include <stdlib.h> /*Header file with general utility functions such as memory management*/

char cMessage[] = "Hello\n"; /*global variable declaration*/

int main (int argc, char **argv) /*One or more functions, each program starts execution at main*/
{
    int i, count; /*Local variable declaration*/

    count = atoi(argv[1]);
    for(i = 0; i < count; i++) {
        printf("Hello %d\n", i);
    }

    return EXIT_SUCCESS;
    
}


