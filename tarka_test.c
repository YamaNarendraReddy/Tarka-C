int printf(char *fmt, ...);
void *tarka_malloc(unsigned long size);
void tarka_free(void *ptr);

void test_temporal() {
    printf("[TARKA] Testing Temporal Safety (Auto-Nullification)...\n");
    char *ptr = tarka_malloc(10);
    ptr[0] = 'H';
    printf("Pointer address: %p\n", ptr);
    
    tarka_free(ptr);
    printf("After tarka_free, pointer address is: %p\n", ptr);
    
    if (ptr == 0) {
        printf("SUCCESS: Pointer was auto-nullified by the Tarka-C compiler!\n");
    } else {
        printf("FAILURE: Pointer still exists! Attempting access to trigger violation...\n");
        char c = ptr[0]; 
    }
}

int main() {
    test_temporal();
    return 0;
}
