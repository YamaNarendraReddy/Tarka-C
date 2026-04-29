int printf(char *fmt, ...);

void exploit_me() {
    char secret[8];
    secret[0] = 'S'; secret[1] = 'E'; secret[2] = 'C'; secret[3] = 'R';
    secret[4] = 'E'; secret[5] = 'T'; secret[6] = '\0';
    
    char buffer[8];

    printf("Secret before overflow: %s\n", secret);
    
    // Manual overflow: write to buffer[10]
    // This should be caught by our Sentry-C!
    printf("Writing to buffer[10]...\n");
    buffer[10] = 'X'; 

    printf("Secret after overflow: %s\n", secret);
}

int main() {
    exploit_me();
    return 0;
}
