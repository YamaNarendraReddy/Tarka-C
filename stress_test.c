#include <string.h>
int printf(char *fmt, ...);

// 1. Global Attack Surface
int global_flag = 0;
char global_buf[8];

void test_global_overflow() {
    printf("[TARKA] Attempting Global Overflow...\n");
    // This should overwrite global_flag in standard C
    // In Tarka-C, this should be trapped!
    global_buf[10] = 1; 
    if (global_flag == 1) printf("Global Flag SMASHED!\n");
}

void test_memcpy_overflow() {
    printf("[TARKA] Attempting Memcpy Overflow...\n");
    char dest[8];
    char *src = "THIS_IS_TOO_LONG_FOR_DEST";
    // Tarka-C intercepts this and checks bounds!
    memcpy(dest, src, 20);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./stress_test [global|memcpy]\n");
        return 1;
    }
    
    if (strcmp(argv[1], "global") == 0) test_global_overflow();
    if (strcmp(argv[1], "memcpy") == 0) test_memcpy_overflow();
    
    return 0;
}
