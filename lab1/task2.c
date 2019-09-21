#include <stdio.h>
#include <libio.h>
#include <string.h>
#include <stdlib.h>


int checkVerbose(int argc, char **argv);

void LSBconvertToBinary(char c, int *binRep);

void RSBconvertToBinary(char c, int *binRep);

void convertToDecimalPrinter(char c,FILE *file);

void binaryPrinter(int *binRep,FILE *file);

void intToCharArr(int iterations, int *from, char *to);


int main(int argc, char **argv) {
    FILE *file = stdout;
    int flagLSBBinary = 0;
    int flagRSBBinary = 0;
    int i = 0;
    for (; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            flagLSBBinary = 1;
        }
        if (strcmp(argv[i], "-b") == 0) {
            flagRSBBinary = 1;
        }
        if (strcmp(argv[i], "-o") == 0) {
            file = fopen(argv[++i], "w");
        }
    }

    char c = (char) getchar();
    int binRep[8];
    while (c != EOF) {
        if (c == 10) { fprintf(file, "\n"); }
        else {
            if (flagLSBBinary == 1) {
                LSBconvertToBinary(c, binRep);
                binaryPrinter(binRep,file);
            } else if (flagRSBBinary == 1) {
                RSBconvertToBinary(c, binRep);
                binaryPrinter(binRep,file);
            } else {
                convertToDecimalPrinter(c,file);
            }
        }


        c = getchar();
    }
    fclose(file);
    return 0;
}

void LSBconvertToBinary(char c, int *binRep) {
    int cVal = c;
    int remain;
    int i;
    for (i = 0; i < 8 && cVal != 10; i++) {
        remain = cVal & 1;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}

void RSBconvertToBinary(char c, int *binRep) {
    int cVal = c;
    int remain;
    int i;
    for (i = 7; i >= 0 && cVal != 10; i--) {
        remain = cVal & 1;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}

void convertToDecimalPrinter(char c, FILE *file) {
    int asciiVal = c;
    if (asciiVal != 10) {
        fprintf(file, "%d ", asciiVal);

    }
}


void binaryPrinter(int *binRep, FILE *file) {
    int i;
    for (i = 0; i < 8; i++) {
        fprintf(file, "%d", binRep[i]);
    }
    fprintf(file, " ");

}

void intToCharArr(int iterations, int *from, char *to) {
    int i;
    for (i = 0; i < iterations; i++) {
        sprintf(&to[i], "%d", from[i]);
    }
    to[8] = 0;
}


