#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef struct {
    char items[MAX_STACK_SIZE];
    int top;
} Stack; //stack structure

void initStack(Stack *s) {
    s->top = -1;
} //initialize stack

int push(Stack *s, char item) {
    if(s->top >= MAX_STACK_SIZE - 1) {
        return 0;
    } //stack overflow

    s->items[++(s->top)] = item;
    return 1; //item inserted into stack
} //insert item into stack

char pop(Stack *s) {
    if(s->top == -1) {
        return '\0';
    }
    return s->items[(s->top)--];
} //pop from of stack

int isBalanced(char* str) {
    Stack stack; 
    initStack(&stack);
    int i = 0;
    char expected; 

    while (*str) {
        if(*str == '(' || *str == '[' || *str == '{') {
            push(&stack, *str);
        } 
        else if(*str == ')' || *str == ']' || *str == '}') {
            if (stack.top == -1) {
                printf("%d: %c\n", i, *str); //empty stack
                return 0;
            }
            expected = pop(&stack);
            if((*str ==')' && expected != '(') || (*str ==']' && expected != '[') || (*str == '}' && expected != '{')) {
                printf("%d: %c\n", i, *str);
                return 0;
            }
        }
        str++;
        i++;
    }
    if(stack.top != -1) {
        printf("open: ");
        while(stack.top != -1) {
            expected = pop(&stack);
            if(expected == '(') {
                printf(")");
            }
            else if (expected == '[') {
                printf("]");
            }
            else if(expected == '{') {
                printf("}");
            }
        }
        printf("\n");
        return 0;
    }
    return 1;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    if (isBalanced(argv[1])) {
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}