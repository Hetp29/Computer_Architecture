#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left; 
    struct Node *right; 
}; //tree node structure

struct Node* create(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node)); //memory for new node
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL; 
    return newNode;
} //create new node

struct Node* insert(struct Node* root, int data) {
    if(root == NULL) {
        printf("inserted\n");
        return create(data);
    }
    if(data < root->data) {
        root->left = insert(root->left, data);
    }
    else if(data > root->data) {
        root->right = insert(root->right, data);
    }
    else {
        printf("not inserted\n");
    }

    return root; 
} //insert new node in BST using recursion 

void search(struct Node* root, int data) {
    if(root == NULL) {
        printf("absent\n");
        return;
    }

    if(data == root->data) {
        printf("present\n");
    }
    else if(data < root->data) {
        search(root->left, data);
    }
    else {
        search(root->right, data);
    }
}

void printTree(struct Node* root) {
    if(root == NULL) {
        return;
    }

    printf("(");
    printTree(root->left); //left subtree
    printf("%d", root->data);
    printTree(root->right); //right subtree
    printf(")");
}

struct Node* max(struct Node* root) {
    while(root->right != NULL) {
        root = root->right;
    }
    return root;
}

struct Node* delete(struct Node* root, int data) {
    if(root == NULL) {
        printf("absent\n");
        return root;
    }

    if(data < root->data) {
        root->left = delete(root->left, data);
    }
    else if(data > root->data) {
        root->right = delete(root->right, data);
    }
    else {
        if(root->left == NULL && root->right == NULL) {
            free(root); //no children
            printf("deleted\n");
            return NULL;
        }
        else if(root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            printf("deleted\n");
            return temp;
        }
        else if(root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            printf("deleted\n");
            return temp;
        }
        else {
            struct Node* maxNode = max(root->left);
            root->data = maxNode->data;
            root->left = delete(root->left, maxNode->data);
            //printf("deleted\n");
        }
    }
    return root;
}

void freeTree(struct Node* root) {
    if(root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    struct Node* root = NULL;
    char command; 
    int value;

    while(scanf(" %c", &command) != EOF) {
        if(command == 'i') {
            scanf("%d", &value);
            root = insert(root, value);
        }
        else if(command == 's') {
            scanf("%d", &value);
            search(root, value);
        }
        else if(command == 'd') {
            scanf("%d", &value);
            root = delete(root, value);
        }
        else if(command == 'p') {
            printTree(root);
            printf("\n");
        }
    }

    freeTree(root);

    return 0;
}