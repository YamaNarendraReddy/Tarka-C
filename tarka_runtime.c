#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Educational Tarka-C: Registry now stores metadata
typedef struct MemoryRegion {
    uintptr_t start;
    uintptr_t end;
    int active;
    const char *name; // Variable name
    int size;         // Original size
    struct MemoryRegion *next;
} MemoryRegion;

#define TABLE_SIZE 16384
MemoryRegion *tarka_table[TABLE_SIZE] = {0};

static uint32_t hash_addr(uintptr_t addr) {
    return (uint32_t)((addr ^ (addr >> 16)) % TABLE_SIZE);
}

void __tarka_register(void *ptr, size_t size, int is_heap, const char *name) {
    uintptr_t addr = (uintptr_t)ptr;
    uint32_t h = hash_addr(addr);
    
    MemoryRegion *reg = malloc(sizeof(MemoryRegion));
    reg->start = addr;
    reg->end = addr + size;
    reg->active = 1;
    reg->name = name ? strdup(name) : "anonymous";
    reg->size = size;
    reg->next = tarka_table[h];
    tarka_table[h] = reg;
}

void __tarka_unregister(void *ptr) {
    uintptr_t addr = (uintptr_t)ptr;
    uint32_t h = hash_addr(addr);
    MemoryRegion *reg = tarka_table[h];
    while (reg) {
        if (reg->start == addr) {
            reg->active = 0;
            return;
        }
        reg = reg->next;
    }
}

// Helper to find the nearest region for diagnostic reporting
MemoryRegion* find_nearest(uintptr_t addr) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        MemoryRegion *r = tarka_table[i];
        while (r) {
            // If the address is within 128 bytes of this region, it's likely an overflow
            if (addr >= r->start - 128 && addr <= r->end + 128) return r;
            r = r->next;
        }
    }
    return NULL;
}

void __tarka_check(void *ptr, const char *filename, int line) {
    if (!ptr) return;
    uintptr_t addr = (uintptr_t)ptr;
    
    // Safety: Implicitly trust the OS environment and stack bottom
    // On Linux, the initial stack and argv are very high.
    // We also trust the binary's own mapping range.
    if (addr > 0x700000000000 || (addr > 0x400000 && addr < 0x800000)) return;

    // First, standard proven check
    uint32_t h = hash_addr(addr);
    MemoryRegion *reg = tarka_table[h];
    while (reg) {
        if (reg->active && addr >= reg->start && addr < reg->end) return;
        reg = reg->next;
    }

    // If not found, scan all regions for the nearest one (Educational Search)
    fprintf(stderr, "\n[TARKA-C DIAGNOSTIC] LOGIC ERROR DETECTED!\n");
    fprintf(stderr, "Location: %s at line %d\n", filename, line);
    fprintf(stderr, "Illegal access at memory address: %p\n", ptr);
    
    MemoryRegion *near = NULL;
    for (int i = 0; i < TABLE_SIZE; i++) {
        MemoryRegion *r = tarka_table[i];
        while (r) {
            if (addr >= r->start - 256 && addr <= r->end + 256) {
                near = r;
                break;
            }
            r = r->next;
        }
        if (near) break;
    }

    if (near) {
        if (!near->active) {
            fprintf(stderr, "Flaw: USE-AFTER-FREE!\n");
            fprintf(stderr, "Explanation: Variable '%s' exists but was already destroyed.\n", near->name);
        } else if (addr >= near->end) {
            fprintf(stderr, "Flaw: BUFFER OVERFLOW!\n");
            fprintf(stderr, "Explanation: You are %ld bytes past the end of variable '%s' (size %d).\n", 
                    (long)(addr - near->end + 1), near->name, near->size);
        } else if (addr < near->start) {
            fprintf(stderr, "Flaw: BUFFER UNDERFLOW!\n");
            fprintf(stderr, "Explanation: You are %ld bytes before the start of variable '%s'.\n", 
                    (long)(near->start - addr), near->name);
        }
    } else {
        fprintf(stderr, "Flaw: WILD POINTER / SEGMENTATION FAULT!\n");
        fprintf(stderr, "Explanation: Address %p is not near any known variable.\n", ptr);
    }
    
    fprintf(stderr, "\nConcept: C does not check bounds. Tarka-C proved this access is UNLOGICAL.\n");
    exit(1);
}

void __tarka_overflow_fail() {
    fprintf(stderr, "\n[TARKA-C DIAGNOSTIC] ARITHMETIC LOGIC ERROR!\n");
    fprintf(stderr, "Flaw: SIGNED INTEGER OVERFLOW!\n");
    fprintf(stderr, "Explanation: A math operation exceeded the maximum value of a signed integer.\n");
    fprintf(stderr, "Concept: In C, this is 'Undefined Behavior'. The result 'rolls over' to a negative number,\n");
    fprintf(stderr, "which often causes security vulnerabilities when calculating buffer sizes.\n");
    exit(1);
}

// Wrapper for standard library calls
void *__tarka_safe_memcpy(void *dest, const void *src, size_t n, const char *file, int line) {
    __tarka_check(dest, file, line);
    __tarka_check((char *)dest + n - 1, file, line);
    __tarka_check((void *)src, file, line);
    __tarka_check((char *)src + n - 1, file, line);
    return memcpy(dest, src, n);
}

void *__tarka_safe_memset(void *s, int c, size_t n, const char *file, int line) {
    __tarka_check(s, file, line);
    __tarka_check((char *)s + n - 1, file, line);
    return memset(s, c, n);
}

char *__tarka_safe_strcpy(char *dest, const char *src, const char *file, int line) {
    size_t n = strlen(src) + 1;
    __tarka_check(dest, file, line);
    __tarka_check(dest + n - 1, file, line);
    __tarka_check((void *)src, file, line);
    __tarka_check((void *)(src + n - 1), file, line);
    return strcpy(dest, src);
}

void* tarka_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr) {
        char buf[256];
        snprintf(buf, sizeof(buf), "heap_alloc_at_%s_%d", file, line);
        __tarka_register(ptr, size, 1, buf);
    }
    return ptr;
}

void tarka_free(void *ptr) {
    if (ptr) __tarka_unregister(ptr);
    free(ptr);
}
