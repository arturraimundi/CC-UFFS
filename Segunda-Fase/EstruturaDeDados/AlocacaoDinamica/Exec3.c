#include <stdio.h>
#include <stdlib.h>
#define ROW 5
#define COL 5
int main(){
   int *p;
   p = (int*) malloc(sizeof(int));
   *p = 22;

   printf("conteudo %d \n", *p);
   printf("endereco %d \n", p);
}