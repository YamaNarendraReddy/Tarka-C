int printf(char *fmt, ...);

int main() {
    int a = 2147483647; // INT_MAX
    int b = 1;
    printf("Trying to add 1 to INT_MAX (%d)...\n", a);
    int c = a + b;
    printf("Result: %d\n", c);
    return 0;
}
