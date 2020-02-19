#include <stdio.h>
#include <stdlib.h>


struct Node{
    int data;
    struct Node* next;
};

struct Node *newNode(int data){
    struct Node *new = malloc(sizeof(struct Node));

    new -> data = data;
    new -> next = NULL;

    return new;
}

struct Node *delete(int data, struct Node *head){
    if (head == NULL){
        return head;
    }

    if (head -> data == data) {
        if (head -> next == NULL) {
            head = head -> next;
            return head;
        }
        head = head -> next;
    }

    struct Node *curr = head -> next;
    struct Node *prev = head;
    while (curr != NULL) { 
        if (curr -> data == data) {
            prev -> next = curr -> next;
        }
        prev = curr;
        curr = curr -> next;
    }

    return head;

}

struct Node *insert(int data, struct Node *head) {
    struct Node *insert = newNode(data);

    if (head == NULL || head -> data > data){
        // puts("head is null\n");
        insert -> next = head;
        head = insert;
    } else if (head -> data == data) {
        free(insert);

    } else {
        struct Node *curr = head;
        while (curr -> next != NULL && curr -> next -> data <= data) {
            if (curr -> next -> data == data) {
                free(insert);
                return head;
            }
            curr = curr -> next;
        }
        insert -> next = curr -> next;
        curr -> next = insert;
    }

    return head;
}

void print_list(struct Node *head) {
    struct Node *curr = head;
    int length = 0;

    while (curr!= NULL) {
        length += 1;
        curr = curr -> next;
    }

    curr = head;
    printf("%d :", length);

    while (curr!= NULL) {
        printf(" %d", curr -> data);
        curr = curr -> next;
    }
    printf("\n");
}


int main(int argc, char **argv) {

    struct Node *head = NULL;
    char operation;
    int data;
    int ret;

    while (1) {

        ret = scanf(" %c %d", &operation, &data);

        if (ret == EOF) {
            break;

        }



        switch (operation) {
            case 'i' :
                head = insert(data, head);
                print_list(head);
                break;

            case 'd' :
                head = delete(data, head);
                print_list(head);
                break;

            default:
                return 0;
        }

    }

    return 0;

}