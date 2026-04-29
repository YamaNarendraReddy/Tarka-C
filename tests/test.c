#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Stack Buffer Overflow
// Overwrites the stack frame, potentially corrupting the return address.
void test_stack_overflow() {
    printf("[*] Testing Stack Buffer Overflow...\n");
    char stack_buf[8];
    // Copying 40 bytes into an 8-byte buffer
    strcpy(stack_buf, "This string is intentionally way too long!"); 
}

// 2. Heap Buffer Overflow
// Corrupts heap metadata by writing past allocated memory.
void test_heap_overflow() {
    printf("[*] Testing Heap Buffer Overflow...\n");
    char *heap_buf = (char *)malloc(10 * sizeof(char));
    if (!heap_buf) return;
    
    // Writing 23 bytes into a 10-byte allocation
    strcpy(heap_buf, "Overwriting heap chunks"); 
    free(heap_buf);
}

// 3. Null Pointer Dereference (Segmentation Fault)
// Attempts to write to address 0x0.
void test_null_dereference() {
    printf("[*] Testing Null Pointer Dereference...\n");
    volatile int *ptr = NULL; // volatile to prevent aggressive compiler optimization
    *ptr = 0xDEADBEEF; 
}

// 4. Writing to Read-Only Memory (Segmentation Fault)
// String literals are stored in the .rodata section. Writing here causes a segfault.
void test_rodata_write() {
    printf("[*] Testing Write to .rodata...\n");
    char *str = "Read Only String Literal";
    str[0] = 'W'; 
}

// 5. Use-After-Free (UAF)
// Accessing heap memory after it has been returned to the OS/allocator.
void test_use_after_free() {
    printf("[*] Testing Use-After-Free...\n");
    int *uaf_ptr = (int *)malloc(sizeof(int));
    if (!uaf_ptr) return;
    
    *uaf_ptr = 42;
    free(uaf_ptr);
    
    // Using the dangling pointer
    *uaf_ptr = 84; 
}

// 6. Double Free
// Corrupts the heap allocator's free list.
void test_double_free() {
    printf("[*] Testing Double Free...\n");
    void *ptr = malloc(32);
    if (!ptr) return;
    
    free(ptr);
    free(ptr); // Double free
}

// 7. Uninitialized Variable Usage
// Reads garbage data from the stack.
void test_uninitialized_vars() {
    printf("[*] Testing Uninitialized Variable Usage...\n");
    volatile int garbage_val;
    if (garbage_val == 42) {
        printf("Lucky number!\n");
    }
}

// 8. Out-of-Bounds Array Access
// Accessing memory outside the defined array boundaries.
void test_out_of_bounds() {
    printf("[*] Testing Out-of-Bounds Array Access...\n");
    int arr[5] = {1, 2, 3, 4, 5};
    volatile int out_of_bounds = arr[10]; // Reading out of bounds
    arr[-1] = 99;                         // Writing out of bounds
}

// 9. Stack Smashing via Recursion (Stack Overflow)
// Exhausts the stack limit (useful to see if your compiler catches deep recursion limits).
void test_infinite_recursion() {
    volatile int dummy = 0;
    test_infinite_recursion();
}

int main(int argc, char *argv[]) {
    printf("=== Custom Compiler Torture Test ===\n");
    printf("WARNING: This program is designed to crash.\n\n");

    if (argc < 2) {
        printf("Usage: %s <test_number>\n", argv[0]);
        printf("1: Stack Overflow\n2: Heap Overflow\n3: Null Deref\n4: RO Data Write\n");
        printf("5: Use-After-Free\n6: Double Free\n7: Uninit Vars\n8: Out of Bounds\n9: Infinite Recursion\n");
        return 0;
    }

    int test_num = atoi(argv[1]);

    switch(test_num) {
        case 1: test_stack_overflow(); break;
        case 2: test_heap_overflow(); break;
        case 3: test_null_dereference(); break;
        case 4: test_rodata_write(); break;
        case 5: test_use_after_free(); break;
        case 6: test_double_free(); break;
        case 7: test_uninitialized_vars(); break;
        case 8: test_out_of_bounds(); break;
        case 9: test_infinite_recursion(); break;
        default: printf("Invalid test number.\n"); break;
    }

    printf("If you see this, the test did not crash successfully.\n");
    return 0;
}
