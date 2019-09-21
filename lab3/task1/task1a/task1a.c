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



int main(int argc, char **argv) {

    FILE *output = stdout;
    struct diff *data1 = malloc(sizeof(diff));
    struct diff *data2 = malloc(sizeof(diff));
    data1->offset= 1;
    data1->orig_value = 1;
    data1->new_value = 1;
    data2->offset = 2;
    data2->orig_value = 2;
    data2->new_value = 2;
    struct node *head = NULL;
    head = list_append(head,data1);
    head = list_append(head,data2);
    list_print(head,output);
    list_free(head);
    return 0;
}

