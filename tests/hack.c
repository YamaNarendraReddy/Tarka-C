#include <stdio.h>

int main() {
    char secret[8] = "SECRET";
    char buffer[8] = "SAFE";

    printf("--- Standard C ---\n");
    printf("Secret before: %s\n", secret);
    
    // Overflowing buffer[10] will hit 'secret' memory
    buffer[10] = 'X'; 
    buffer[11] = 'I';
    buffer[12] = 'L';
    buffer[13] = '\0';

    printf("Secret after:  %s (CORRUPTED!)\n", secret);
    return 0;
}
