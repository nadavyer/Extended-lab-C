#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




typedef struct diff {
    long offset; /* offset of the difference in file starting from zero*/
    unsigned char orig_value;     /* value of the byte in ORIG */
    unsigned char new_value;     /* value of the byte in NEW */
} diff;

typedef struct node node;

struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};

/* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE.
Each item followed by a newline character. */
void list_print(node *diff_list, FILE *output) {
    node *curr = diff_list;
    while (curr) {
        fprintf(output, "byte %li %02X %02X\n", curr->diff_data->offset, curr->diff_data->orig_value,
                curr->diff_data->new_value);
        curr = curr->next;
    }
}

/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/

node *list_append(node *diff_list, diff *data) {
    struct node *new_node = malloc(sizeof(node));
    new_node->diff_data = data;
    new_node->diff_data->offset = data->offset;
    new_node->diff_data->orig_value = data->orig_value;
    new_node->diff_data->new_value = data->new_value;
    new_node->next = diff_list;
    return new_node;

}

void list_free(node *diff_list) {
    node *curr = diff_list;
    node *tmp;
    while (curr) {
        tmp = curr;
        curr = curr->next;
        free(tmp->diff_data);
        free(tmp);
    }
}

void printHex(char *buffer, long size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%02x ", (unsigned char) buffer[i]);
    }
    printf("\n");
}

int checkStringRepNum (char *string) {
    int i;
    for (i = 0; i < strlen(string); i++) {
        if (isdigit(string[i]) == 0) {
            return 0;
        }
    }
    return 1;
}


long getFileSizeBytes(FILE *f) {
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    return fsize;
}

long minByteSize(long originByteSize, long newByeSize) {
    if (originByteSize < newByeSize) {
        return newByeSize;
    } else {
        return originByteSize;
    }
}

int diffByte(unsigned char originByte, unsigned char newByte) {
    if (originByte == newByte) {
        return 0;
    } else {
        return 1;
    }
}

int countLines(FILE *file) {
    int output = 0;
    char c;
    while (!feof(file)) {
        c = fgetc(file);
        if (c == '\n') {
            output++;
        }
    }
    return output;
}



int main(int argc, char **argv) {

    FILE *output = stdout;
    char *originFileName = NULL;
    char *newFileName = NULL;
    FILE *readFromFile = NULL;
    FILE *originFile = NULL;
    FILE *newFile = NULL;
    unsigned char origingNextByte = '\0';
    unsigned char newNextByte = '\0';
    struct node *head = NULL;
    int diffCounter = 0;
    int tFlag = 0;
    int iFlag = 0;
    int rFlag = 0;
    int originSaved = 0;
    int newSaved = 0;
    int i, j;
    

    for (j = 1; j < argc; j++) {
        if (strcmp(argv[j], "-o") == 0) {
            output = fopen(argv[j + 1], "w");
            j++;
        } else if (strcmp(argv[j], "-t") == 0) {
            tFlag = 1;
        } else if (strcmp(argv[j], "-i") == 0) {
            iFlag = 1;
            readFromFile = fopen(argv[j + 1], "r+");
        }else if (originSaved == 0) {
            originFileName = argv[j];
            originFile = fopen(originFileName, "rb+");
            originSaved = 1;
        } else if (newSaved == 0) {
            newFileName = argv[j];
            newFile = fopen(newFileName, "rb+");
            newSaved = 1;
        }
}
    
        
    if (iFlag == 1) {
        fprintf(output, "%d", countLines(readFromFile));
    }
    

    else {
        long originByteSize = getFileSizeBytes(originFile);
        long newByteSize = getFileSizeBytes(newFile);
        long shortestFileBytes = minByteSize(originByteSize, newByteSize);
        for (i = 0; i < shortestFileBytes; i++) {
            if ((fread(&origingNextByte, 1, 1, originFile) == 1) && (fread(&newNextByte, 1, 1,newFile) == 1)) { 			
                if (diffByte(origingNextByte, newNextByte)) {
                    struct diff *diffToInsert = malloc(sizeof(diff));
                    diffToInsert->offset = i;
                    diffToInsert->orig_value = origingNextByte;
                    diffToInsert->new_value = newNextByte;
                    head = list_append(head, diffToInsert);
                    diffCounter++;
                }
            }
        }
    }
    

    if (tFlag == 1 && iFlag == 0) {
        fprintf(output, "%d\n", diffCounter);
    } else if (rFlag != 1) {
        list_print(head, output);
    }
    if(iFlag) {
        fclose(readFromFile);
    }
    fclose(originFile);
    fclose(newFile);
    list_free(head);
    fclose(output);
    return 0;
}










