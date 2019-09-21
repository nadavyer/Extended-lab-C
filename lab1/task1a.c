#include <stdio.h>
#include <libio.h>

int main(int argc, char **argv) {
     char nextChar = getchar();
    while (nextChar != EOF) {

            int asciiVal = nextChar;
            if (asciiVal != 10) {
                printf("%d ", asciiVal);
            }
            else {
                printf("\n");
            }

        nextChar = getchar();
    }
    return 0;
}
