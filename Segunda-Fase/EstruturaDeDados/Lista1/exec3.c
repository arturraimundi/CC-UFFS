#include <stdio.h>

int main(){
    int inicio;
    int final;
    int total;

    printf("Inicio: ");
    scanf("%d", &inicio);

    printf("final: ");
    scanf("%d",&final);

    if(final < inicio){
        inicio = 24 - inicio;
        total = inicio + final;
    } else{
        total = final - inicio;
    };

    printf("%d horas \n", total);
}