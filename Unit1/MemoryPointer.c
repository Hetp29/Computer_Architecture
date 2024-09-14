#include <stdio.h>

void SwapByValue(int firstVal, int secondVal) {
    int temp = firstVal;
    firstVal = secondVal;
    secondVal = temp;
}

void SwapByReference(int *firstVal, int *secondVal) {
    int temp = *firstVal;
    *firstVal = *secondVal;
    *secondVal = temp;
}

int main() 
{
    int fv = 6, sv = 10;

    printf("Before SwapByValue: fv = %d, sv = %d\n", fv, sv);
    SwapByValue(fv, sv);
    printf("After SwapByValue: fv = %d, sv = %d\n", fv, sv);

    printf("Before SwapByReference: fv = %d, sv = %d\n", fv, sv);
    SwapByReference(&fv, &sv);
    printf("After SwapByReference: fv = %d, sv = %d\n", fv, sv);

    return 0;
}