#include <stdio.h>

int main() {
    int horas;
    float valor;
    printf("Horas Trabalhadas:");
    scanf("%d", &horas);
    printf("Valor hora:");
    scanf("%f", &valor);
    printf("%f\n", horas * valor);

}