int fibonacci(int n) {
    if (n < 2) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    printf("Fibonacci: %d\n", fibonacci(10));
    printf("Fibonacci 15: %d\n", fibonacci(15));
    return 0;
}
