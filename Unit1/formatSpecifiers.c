#include <stdio.h>

int main() {
    int age = 25;
    float height = 5.9;
    char name[] = "John";

    printf("Name: %s\n, Age: %d\n, Height: %f\n", name, age, height);

    return 0;
}

//%s is replaced by John
//%d is replaced by 25
//%f is replaced by 5.900000
//\n moves to next line (new line)