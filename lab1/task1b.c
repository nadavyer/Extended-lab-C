#include <stdio.h>
#include <libio.h>
#include <string.h>
#include <stdlib.h>


unsigned int checkVerbose(int argc, char **argv);

void LSBconvertToBinary(unsigned char c, int *binRep);

void convertToDecimalPrinter(unsigned char c);

void binaryPrinter(int *binRep);


int main(int argc, char **argv) {
    unsigned int verbose = checkVerbose(argc, argv);
    char c = getchar();
    while (c != EOF) {
        if (verbose == 0) { convertToDecimalPrinter(c); }
        else if (verbose == 1) {
            int binRep[8];
            LSBconvertToBinary(c,binRep);
            if (c !=10) {
                binaryPrinter(binRep);}
            else {
                printf("\n");
            }
        }
        c = getchar();
    }
    return 0;
}

unsigned int checkVerbose(int argc, char **argv) {
    unsigned int verbose = 0;
    int i;
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-l") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i],"-b") == 0) {
            verbose = 2;
        }
    }
    return verbose;
}


void LSBconvertToBinary(unsigned char c, int *binRep) {
    unsigned int cVal = c;
    unsigned int remain;
    int i;
    for (i = 0; i < 8 && cVal != 10; ++i) {
        remain = cVal & 1 ;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}



void convertToDecimalPrinter(unsigned char c) {
    int asciiVal = c;
    if (asciiVal != 10) {
        printf("%d ", asciiVal);
    } else {
        printf("\n");
    }
}

void binaryPrinter(int *binRep) {
    int i;
    for(i = 0; i < 8; i++) {
        printf("%d", binRep[i]);
    }
    printf(" ");
}


