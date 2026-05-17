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
//tarefa - ler uma lista linkada e criar uma nova lista com apenas os valores inteiros

int main(){
 Node* lista = NULL;

 insertFim(lista, 3);
 insertFim(lista, 4);
 insertFim(lista, 9);
 insertFim(lista, 8);
while(lista -> next != NULL){
   Node* atual = lista; 
   printf("%d", lista -> data);
   lista = atual-> next;
}

}