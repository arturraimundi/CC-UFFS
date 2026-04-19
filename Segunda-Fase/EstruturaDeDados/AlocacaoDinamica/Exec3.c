#include <stdio.h>
#include <stdlib.h>



typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* insertFim(Node* head, int valor) {
    Node* novo = (Node*) malloc(sizeof(Node));
    novo->data = valor;
    novo->next = NULL;

    if (head == NULL) return novo;

    Node* atual = head;
    while (atual->next != NULL) {
        atual = atual->next;
    }

    atual->next = novo;
    return head;
}

int main(){
   
}