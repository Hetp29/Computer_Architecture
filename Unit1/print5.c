#include <stdio.h>

int main() {
    for(int i = 1; i <= 5; i++) {
        printf("%d\n", i);
    }

    return 0;
}

//Compile: gcc print5.c -o print5
//-o print5 part is where we specify name of output executable 
//without -o in C, we will get an a.out file
//use -o when you have multiple files in directory so it doesn't get overwritten 

//%d, %f, and %c are format specifiers that tell printf() how to format and display 
//variables (like numbers or strings)

//\n means new line, cursoer moves to next line after printing
//example: printf("Hello\n");
//Output: Hello
//(cursor moves to the next line)