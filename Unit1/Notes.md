Variable Declarations
-Variables are used as names for data items
-Variables have type, which tells the compiler how the data is to be interpreted (and how much space it needs, etc.)
ex. int counter;
ex. int startPoint;
Variables can be global or local 
Global: These variables are declared outside scope of any function from anywhere
Local: declare inside scope of a function accessible only from inside of function 

Basic Data Types:
Char: individual characters ('a', 'b', '\t', '\n')
int: integers (-15, 0, 35)
float: real numbers (-23.6, 0, 4.56)
double: real numbers with double precisions (-23.6, 0, 4.56)

Modifiers:
short, long: control size/range of numbers
signed, unsigned: include negative numbers or not

Arithmetic Operators:
* is multiply (x * y)
/ is divide (x / y)
% is modulo (x % y)
+ is addition (x + y)
- is subtraction (x - y)
* / % have higher precedence than +- 
Rule of thumb: remember only a few precedence rules
Use () for everything else

Special Operators: ++ and --
-Changes values of variable before (or after) its value is used in an expression 
Symbol:
++ (Postincrement is x++ and preincrement is ++x)
-- (Postdecrement is x-- and predecrement is --x)
Pre: Increment/decrement variable before using its value
Post: Increment/decrement variable after using its value

Relational Operators
Symbol:
> is greater than (x > y)
>= is greater than or equal to (x >= y)
< is less than (x < y)
<= is less than or equal to (x <= y)
== is equal to (x == y)
!= is not equal to (x != y)

Logic Operators
Symbol:
! is logical NOT (!x)
&& is logical AND (x && y)
|| is logical OR (x || y)
Result is 1(TRUE) or 0(FALSE)

Bit Operators
Symbol:
~ is complement (~x)
& is bit AND (x & y)
| is bit OR (x | y)
Operate on bits of variables or constants 
For example:
~0101 = 1010
0101 & 1010 = 0000
0101 | 1010 = 1111

Expressions and Assignments

Expression is a "a computation" with a result
(x + y) * z
int x, z, float y
The result of operation above will be a float because of type promotion during arithmetic operations

Assignment statement itself is an expression and has a value, in below line, value of result is assigned to x
x = (x + y) * z

Control Statements
Conditional:
-if else
-switch
Iteration (loops) 
-while
-for
-do while
Specialized "go-to" 
-break
-continue

The if Statement
if (expression-1) {statements-1}
else if (expression-2) {statements-2}
else if (expression-n-1) {statements-n-1}
else {statements-n}
if (condition) {
    //code to execute if condition is true
}
if (num > 0) {
        printf("The number is positive.\n");
    } else if (num < 0) {
        printf("The number is negative.\n");
    } else {
        printf("The number is zero.\n");
    }
    return 0;


The switch Statement
switch(expressions) {
    case const - 1: statements - 1;
    case const - 2: statements - 2;
    default: statements - n;
}
Evaluates expression; results must be integer
Finds 1st "case" with matching contant 
-executes corresponding statements
-continue executing until encounter a break or end of switch statement
"default" always matches
#include <stdio.h>

int main() {
    int number = 2;

    switch (number) {
        case 1:
            printf("One\n");
        case 2:
            printf("Two\n");
        case 3:
            printf("Three\n");
        default:
            printf("Default\n");
    }

    return 0;
}
Two
Three
Default
-since number is 2, it prints "Two" but also "Three" and "Default" because there are no break statements
-switch statement is for selecting one of several fixed options

Statement:

while(expressions){...}: This is while loop and will execute zero or more times, while it is not equal to 0 and it inside block of code will be computed BEFORE each iteration 

do{...} while(expression) repeats set of statements one or more times, while expression != 0, and compute expression AFTER each iteration

for(start-expression; conditional-expression; update-expression){...}: zero or more times, while cond-expression != 0, compute start-expression before first iteration, compute update-expression after each iteration

Specialized Go-to's
break:
-Force immediate exit from switch or loop
-go-to statement after following loop
continue:
-skip rest of computation in the current iteration of a loop 
-go-to evalutation of conditional expression for execution next iteration 
once the break statement is executed, the loop terminates immediately and permanently for that particular execution of the program. The loop will not restart or continue from where it left off.

int index = 0;
int sum = 0;
while ((index >= 0) && (index <= 20))
{
index += 1;
if (index == 11) break;
if ((index % 2) == 1) continue;
sum = sum + index;
}
This code adds even numbers to sum 

Function Calls
Function call as part of an expression:
x + Factorial(y)
arguments evaluated before function call 
-multiple arguments: no defined order or evaluation 
returned value is used to compute expression 
cannot have a void return type
Function call as a statement:
Factorial(y);
can have a void return type
returned value is discarded (if there is one)

Function call as a statement:
#include <stdio.h>

void great() {
    print("Hello, World!\n");
}

int main() {
    greet();
    return 0;
}

Function call as a part of expression:
#include <stdio.h>

int square(int x) {
    return x * x;
}

int main() {
    int result = 5 + square(3);
    printf("Result: %d\n", result);
    return 0;
}

Function Prototypes
Can declare functions without specifying implementation 
int Factorial(int)
-can specify parameter names but don't have to
-this is called function signature
Declarations allow functions to be "used" without having the implementation until link time
■ Separate compilation
● Functions implemented in different files
● Functions in binary libraries
■ Signatures are often given in header files
● E.g., stdio.h gives the signatures for standard I/O functions

Input and Output
Variety of I/O functions in C standard library 
#include <stdio.h>
printf("%d\n", counter);
String contains characters to print and formatting directives for variables 
This call will print variable counter as a decimal integer, followed by linefeed(\n)
scanf("%d", &startPoint);
■ String contains formatting directives for parsing input
■ This call says to read a decimal integer and assign it to the
variable startPoint. (Don't worry about the & yet.)

Memory 
C's memory model matches underlying (virtual) memory system 
