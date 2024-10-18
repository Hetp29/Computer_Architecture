#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(!newNode) {
        printf("Memory allocation failed\n");
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insert(Node** head, int data) {
    

    if(*head == NULL || (*head)->data > data) {
        if(*head != NULL && (*head)->data == data) {
            return;
        }
        Node* newNode = createNode(data);
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Node* current = *head;
    while(current->next != NULL && current->next->data < data) {
        current = current->next;
    }

    if(current->data == data) {
        return;
    }

    if(current->next != NULL && current->next->data == data) {
        return;
    }

    Node* newNode = createNode(data);
    newNode->next = current->next;
    current->next = newNode;
}

void delete(Node** head, int data) {
    if(*head == NULL) return;

    Node* current = *head;
    Node* prev = NULL;

    while(current != NULL && current->data < data) {
        prev = current; 
        current = current->next;
    }

    if(current != NULL && current->data == data) {
        if(prev == NULL) {
            *head = current->next;
        }
        else {
            prev->next = current->next;
        }
        free(current);
    }
}

void printList(Node* head) {
    Node* current = head;
    int count = 0;

    while (current != NULL) {
        if(count == 0) {
            printf("%d", current->data);
        }
        else {
            printf(" %d", current->data);
        }
        current = current->next;
        count++;
    }

    printf("\n");
} 


int main() {
    Node* head = NULL;
    char command;
    int value;

    while (scanf(" %c %d", &command, &value) != EOF) {
        if (command == 'i') {
            insert(&head, value);
        }
        else if (command == 'd') {
            delete(&head, value);
        }
        else {
            printf("Invalid command\n");
            continue;
        }

        int length = 0;
        Node* current = head;
        while (current != NULL) {
            length++;
            current = current->next;
        }

        if (length == 0) {
            printf("%d :", length);
        } 
        else {
            printf("%d : ", length);
        }
        printList(head);
    }

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
