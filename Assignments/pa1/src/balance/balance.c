//checking for balanced parentheses and brackets in string 
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack{
    char *items; //points to dynamically allocated array, stack is not fixed size it is dynamically allocated based on capacity
    int top;
    int capacity; 
} Stack; //typename

Stack* createStack(int capacity) {
    Stack *s = (Stack*)malloc(sizeof(Stack)); //allocate memory for stack structure
    s->items = (char*)malloc(capacity * sizeof(char)); //allocate memory for cstack items
    s->top = -1; 
    s->capacity = capacity; //initial capacity
    return s; 
}

int push(Stack *s, char item) {
    if (s->top >= s->capacity - 1) {
        s->capacity *= 2;//stack is full and need to resize, double capacity
        s->items = (char*)realloc(s->items, s->capacity *sizeof(char));//can dynamically resize to accomodate items without losing existing data
        if(s->items == NULL) {
            return 0;
        }
    }
    s->items[++(s->top)] = item; //item is added to stack
    return 1; 
}

char pop(Stack *s) {
    if (s->top == -1) {
        return '\0'; //return null if empty 
    }
    return s->items[(s->top)--]; //retrieve top item then decrease top index to pop
}

int isBalanced(char* str) {
    Stack *stack = createStack(100);  //initially size 100
    int i = 0;
    char expected; //expected matching character 

    while (*str) {
        if (*str == '(' || *str == '[' || *str == '{') {
            push(stack, *str);  //push opening brackets to stack
        } 
        else if (*str == ')' || *str == ']' || *str == '}') {
            if (stack->top == -1) {
                printf("%d: %c\n", i, *str); 
                free(stack->items); //free stack memory 
                free(stack); 
                return 0; //no matching opening bracket and unbalanced
            }
            expected = pop(stack); //pop items and check if they match current closing bracket 
            if ((*str == ')' && expected != '(') || (*str == ']' && expected != '[') || (*str == '}' && expected != '{')) {
                printf("%d: %c\n", i, *str); 
                free(stack->items); 
                free(stack); 
                return 0;
            }
        }
        str++;
        i++;
    }

    if (stack->top != -1) { 
        printf("open: ");
        while (stack->top != -1) {
            expected = pop(stack);
            //print matching closing bracket
            if (expected == '(') {
                printf(")");
            } else if (expected == '[') {
                printf("]");
            } else if (expected == '{') {
                printf("}");
            }
        }
        printf("\n");
        free(stack->items); 
        free(stack); 
        return 0;
    }

    free(stack->items); 
    free(stack); 
    return 1; 
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE; 
    }

    if (isBalanced(argv[1])) {
        return EXIT_SUCCESS; 
    } else {
        return EXIT_FAILURE; 
    }
}
