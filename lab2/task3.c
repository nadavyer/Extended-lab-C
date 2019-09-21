#include <stdio.h>
#include <string.h>

struct fun_desk {
    char *name;
    void (*fun)(char);
};

void echo_printer(char c);

void ascii_printer(char c);

void binary_printer(char c);

void binary_printerML(char c);

void string_reader(char* s);

void bitwise_or(char* s);



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
        fgets(s, 11, stdin);
    }


void bitwise_or(char* s){
    char result = s[0];
    int i;
    for (i = 1; i < strlen(s); i++) {
        result |= s[i];
    }
    ascii_printer(result);
    binary_printer(result);
}



int main(int argc, char **argv){
    struct fun_desk menu[] ={{"echo printer", echo_printer},
                            {"ascii printer",ascii_printer},
                            {"binary printer (lsb)", binary_printer},
                            {"binary printer (rsb)", binary_printerML},
    };
    printf("Please enter a string (0<size<=10):\n");
    char userInput[11];
    string_reader(userInput);
    printf("Please choose printer type:\n");
    int i;
    for (i = 0; i < 4; i++) {
        printf("%d) %s\n",i,menu[i].name);
    }
    printf("%d) bitwise and\n", i);
    
    int optionNum;
    printf("option: ");
   
    while (scanf("%d",&optionNum) != EOF ) {
        if (optionNum == 4) {
            bitwise_or(userInput);
            printf("\n");
        } else {
            string_printer(userInput, menu[optionNum].fun);
        }
        printf("\noption: ");
        
    }
    printf("DONE.\n");
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
    if (c !=10){
        int binRep[9];
        LSBconvertToBinary(c,binRep);
        binaryPrinter(binRep);
    }
}

void binary_printerML(char c){
    if (c !=10){
        int binRep[9];
        RSBconvertToBinary(c,binRep);
        binaryPrinter(binRep);
    }
}

void LSBconvertToBinary( char c, int *binRep) {
     int cVal = c;
     int remain;
    int i;
    for (i = 0; i < 9 && cVal != 10; i++) {
        remain = cVal & 1 ;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}

void RSBconvertToBinary(char  c, int *binRep) {
    int cVal = c;
    int remain;
    int i;
    for (i = 8; i >= 0 && cVal != 10; i--) {
        remain = cVal & 1 ;
        binRep[i] = remain;
        cVal = cVal >> 1;
    }
}

void binaryPrinter(int *binRep) {
    int i;
    for(i = 0; i < 9; i++) {
        printf("%d", binRep[i]);
    }
    printf(" ");
}

