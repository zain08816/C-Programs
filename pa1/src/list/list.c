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
            head == head -> next;
            return head;
        }
        head = head -> next;
    }

    struct Node *curr = head -> next;
    struct Node *prev = head;
    while (curr != NULL) { 
        if (curr -> data == data) {
            prev -> next = curr -> next;
            free(curr);
        }
        prev = curr;
        curr = curr -> next;
    }

    return head;

}

struct Node *insert(int data, struct Node *head) {
    struct Node *insert = newNode(data);

    if (head == NULL || head -> data >= insert -> data){
        insert -> next = head;
        head = insert;
    } else {
        struct Node *curr = head;
        while (curr -> next != NULL && curr -> next -> data <= insert -> data) {
            if (curr -> data == insert -> data) {
                return head;
            }
            curr = curr -> next;
        }

        insert -> next = curr -> next;
        curr -> next = insert;
    }

    return head;
}


int main(int argc, char **argv) {


    
}