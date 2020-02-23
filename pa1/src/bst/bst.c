#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node *newNode(int data){
    struct Node *new = malloc(sizeof(struct Node));

    new -> data = data;
    new -> left = NULL;
    new -> right = NULL;

    return new;
}

struct Node *insert(int data, struct Node *root) {
    

    if (root == NULL){
        printf("inserted\n");
        return newNode(data);
    }
    if (data == root -> data){
        printf("not inserted\n");
        return root;
    }
    if (data < root -> data){
        root -> left = insert(data, root -> left);
    } else if (data > root -> data){
        root -> right = insert(data, root -> right);
    }

    // printf("not inserted");
    return root;
}

int search(int data, struct Node *root) {

    if (root == NULL) {
        return 0;
    }

    if (root -> data == data) {
        return 1;

    } else if (root -> data > data) {

        if (root -> left == NULL) {
            return 0;
        } else {
            return search(data, root -> left);
        }

    } else if (root -> data < data) {

        if (root -> right == NULL) {
            return 0;
        } else {
            return search(data, root -> right);
        }
        
    }
  
    return 0;

}

 void print_tree(struct Node *root) {
    if (root == NULL) {
        return;
    }

    if (root -> left != NULL) {
        printf("%c", '(');
        print_tree(root -> left);
        printf("%c", ')');
    }

    printf("%d", root -> data);

    if (root -> right != NULL){
        printf("%c", '(');
        print_tree(root -> right);
        printf("%c", ')');
    }
    
}

struct Node *smallest_node(struct Node *root){
    struct Node *curr = root;

    while (curr && curr -> left != NULL) {
        curr = curr -> left;
    }
    return curr;
}

struct Node *delete(int data, struct Node *root) {
    if (root == NULL) {
        printf("absent\n");
        return root;
    }
    if ( root -> data > data) {
        root -> left = delete(data, root -> left);

    } else if ( root -> data < data) {
        root -> right = delete(data, root -> right);

    } else if ( root -> data == data) {
        // 0
        if (root -> left == NULL && root -> right == NULL) {
            free(root);
            printf("deleted\n");
            return NULL;
        // 1
        } else if (root -> left == NULL || root -> right == NULL) {

            struct Node *hold;
            if (root -> left == NULL) {
                hold = root->right;
                free(root);
                printf("deleted\n");
                return hold;
            } else if (root -> right == NULL) {
                hold = root -> left;
                free(root);
                printf("deleted\n");
                return hold;
            }
        // 2
        } else {
            struct Node *hold = smallest_node(root -> right);
            root -> data = hold -> data;

            root -> right = delete(hold -> data, root -> right);
        }

    }

    return root;


}



int main(int argc, char **argv) {

    struct Node *root = NULL;
    char operation;
    int data;
    // int ret;

    while (1) {
        if (scanf(" %c", &operation) == EOF) {return 0;}

        switch (operation) {
            case 'i' :
                if (scanf(" %d", &data) == EOF) {return 0;}
                root = insert(data, root);
                break;

            case 'd' :
                if (scanf(" %d", &data) == EOF) {return 0;}
                root = delete(data, root);
                break;

            case 's' :
                if (scanf(" %d", &data) == EOF) {return 0;}
                if (search(data, root)){
                    printf("present\n");
                } else {
                    printf("absent\n");
                }
                break;
                
            case 'p' :
                if (root == NULL) {
                    printf("\n");
                    break;
                }
                printf("%c", '(');
                print_tree(root);
                printf("%c", ')');
                printf("\n");
                break;

        }

    }

    return 0;

}