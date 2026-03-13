#include <stdio.h>

int main(){
    int numerador;
    scanf("%d", &numerador);
    
    for(int i = numerador -1; i > 1; i = i -1){
        numerador = numerador * i;
    }
    printf("%d", numerador);
}