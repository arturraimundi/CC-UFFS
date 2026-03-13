#include <stdio.h>

int main(){

    int matricula;
    float nota;
    float media;
    float mediaI;
    int rTotal;

    printf("Matricula: ");
    scanf("%d", &matricula);

    while(matricula != 0){
        mediaI = 0;
        for(int i = 1; i <= 5; i++){       
            printf("Nota %d:", i);
            scanf("%f", &nota);
            media += nota;
            mediaI += nota;
            rTotal ++;
        }

        printf("Matricula: %d \n", matricula);
        printf("Média individual: %f \n", mediaI/5);
        
        printf("Matricula: ");
        scanf("%d", &matricula);
    }
    

    media = media/rTotal;

    printf("Média = %f", media);

}