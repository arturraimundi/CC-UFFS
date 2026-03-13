#include <stdio.h>

int main(){
    int numerador;
    int multiplicador;
    int adicao;
    printf("Numerador: ");
    scanf("%d", &numerador);
    adicao = numerador;
    printf("multiplicador: ");
    scanf("%d", &multiplicador);
    printf("%d \n", numerador);

    while(multiplicador > 1){
        numerador = adicao + numerador;
        multiplicador = multiplicador - 1;
        printf("%d \n", numerador);
    }
}