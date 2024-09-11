int factorial(int n)
{
    if (n <= 1) return 1;

    return n * factorial(n - 1);
}

int odd(int);

int even(int n)
{
    if(n == 0) return 1;
    return !odd(n - 1);
}

int odd(int n)
{
    return !even(n - 1);
}