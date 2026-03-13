#include <stdio.h>

int main(){

    float preco;
    int escolha;


    printf("Preco do podruto:");
    scanf("%f", &preco);
    printf("forma de pagamento:");
    scanf("%d", &escolha);
    if(escolha == 1){
        printf("preco a vista: %f", preco*0.95);
    
    } else if(escolha == 2){
        printf("preco a prazo: %f", preco*1.1);

    }
}