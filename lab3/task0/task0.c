#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHex(char *buffer, long size) {
    int i;
    for (i = 0; i < size ; i++) {
        printf("%02x ",(unsigned char)buffer[i]);
    }
    printf("\n");
}


int main(int argc, char **argv) {

    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    printHex(string,fsize);
    fclose(f);

    string[fsize] = 0;
    free(string);
    return 0;
}


