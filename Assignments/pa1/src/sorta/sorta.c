//strcmp() is for string comparison and uses lexicographical ordering
//based on ASCII comparisons, 'Z' comes before 'a'
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strcmp()

void sortStrings(char *arr[], int n) {
    //sorting algorithm 
    char *temp;
    //bubble sort, largest element is placed in correct position (end of the array), then second largest, etc.
    for(int i = 1; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(strcmp(arr[i], arr[j]) > 0) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

    }


}

int main(int argc, char *argv[]) {
    //argc is how many command lines were passed to program (including program name itself)
    //argv[] is array of char pointers where each element points to one command-line argument
    //argc: how many elements are in argv and how many strings need to be stored
    //argv provides data to be sorted (array of strings
    //argv[0] is name or path of the program and not one of the arguments provided by the user
    if (argc == 1) {
        //printf("No words to be printed. \n");
        return EXIT_FAILURE;
        return 0;
    }

    sortStrings(argv, argc);

    for(int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

}