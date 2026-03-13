#include <stdio.h>

int main(){
    int salario;
    float salarios;
    int filho;
    float filhos;
    int tRodadas;

    printf("salario: ");
    scanf("%d", &salario);
    printf("filhos: ");
    scanf("%d", &filho);

    while(salario > 0){
        tRodadas = tRodadas + 1;
        salarios = salarios + salario;
        filhos = filhos +  filho;

        printf("salario: ");
        scanf("%d", &salario);
        printf("filhos: ");
        scanf("%d", &filho);
        

    }
    salarios = salarios / tRodadas;
    filhos = filhos / tRodadas;
    printf("%f", salarios);
    printf("média salarial: %f \n", salarios);
    printf("média de filhos: %f \n", filhos);
   
}