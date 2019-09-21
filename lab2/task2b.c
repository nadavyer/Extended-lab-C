#include <stdio.h>
#include <string.h>


void echo_printer(char c);

void ascii_printer(char c);

void binary_printer(char c);

void binary_printerML(char c);



void LSBconvertToBinary(char c, int *binRep);

void RSBconvertToBinary(char c, int *binRep);

void binaryPrinter(int *binRep);



void string_printer(char* str, void (*f) (char)){
    int i;
    for (i = 0; i < strlen(str) ; i++) {
        f(str[i]);
    }
}

void string_reader(char* s){
    /* TODO: Complete during task 2c*/
}

void bitwise_or(char* s){
    /*TODO: Complete during task2c */
}



int main(int argc, char **argv){
    string_printer("hello",binary_printer);
    return 0;
}



void echo_printer(char c) {
    printf("%c",c);
}

void ascii_printer(char c) {
    int asciiVal = c;
    if (asciiVal != 10) {
        printf("%d ", asciiVal);
    } else {
        printf("\n");
    }
}

void binary_printer(char c) {
    int binRep[8];
    LSBconvertToBinary(c,binRep);
    binaryPrinter(binRep);
}

void binary_printerML(char c){
    int binRep[8];
    RSBconvertToBinary(c,binRep);
    binaryPrinter(binRep);
    }

void LSBconvertToBinary( char c, int *binRep) {
    unsigned int cVal = c;
    unsigned int remain;
    int i;
    for (i = 0; i < 8 && cVal != 10; ++i) {
        remain = cVal & 1 ;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}

void RSBconvertToBinary(char  c, int *binRep) {
    int cVal = c;
    int remain;
    int i;
    for (i = 7; i >= 0 && cVal != 10; --i) {
        remain = cVal & 1 ;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}

void binaryPrinter(int *binRep) {
    int i;
    for(i = 0; i < 8; i++) {
        printf("%d", binRep[i]);
    }
    printf(" ");
}

