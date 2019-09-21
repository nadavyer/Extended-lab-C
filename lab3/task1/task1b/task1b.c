#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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
void list_print(node *diff_list,FILE* output){
    node *curr = diff_list;
    while (curr) {
        fprintf(output,"byte %li %02X %02X\n", curr->diff_data->offset, curr->diff_data->orig_value, curr->diff_data->new_value);
        curr = curr->next;
    }
}

/* Add a new node with the given data to the list,
   and return a pointer to the list (i.e., the first node in the list).
   If the list is null - create a new entry and return a pointer to the entry.*/

node* list_append(node* diff_list, diff* data){
     struct node* new_node = malloc(sizeof(node));
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
    for (i = 0; i < size ; i++) {
        printf("%02x ",(unsigned char)buffer[i]);
    }
    printf("\n");
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
    }
    else {
        return originByteSize;
    }
}

int diffByte (unsigned char originByte, unsigned char newByte) {
    if (originByte == newByte) {
        return 0;
    }
    else {
        return 1;
    }
}





int main(int argc, char **argv) {
    FILE *output = stdout;
    char *originFileName = argv[1];
    char *newFileName = argv[2];
    FILE *originFile = fopen(originFileName, "rb");
    FILE *newFile = fopen(newFileName, "rb");
    unsigned char origingNextByte;
    unsigned char newNextByte;
    struct node *head = NULL;

    long originByteSize = getFileSizeBytes(originFile);
    long newByteSize = getFileSizeBytes(newFile);
    long shortestFileBytes = minByteSize(originByteSize, newByteSize);
    int i;

    for (i = 0; i < shortestFileBytes; i++) {
        if ((fread(&origingNextByte, 1, 1, originFile) == 1) && (fread(&newNextByte, 1, 1, newFile) == 1)) {
            if (diffByte(origingNextByte, newNextByte)) {
                struct diff *diffToInsert = malloc(sizeof(diff));
                diffToInsert->offset = i;
                diffToInsert->orig_value = origingNextByte;
                diffToInsert->new_value = newNextByte;
                head = list_append(head,diffToInsert);
            }
        }
    }
    fclose(originFile);
    fclose(newFile);
    list_print(head,output);
    list_free(head);
    return 0;
}










