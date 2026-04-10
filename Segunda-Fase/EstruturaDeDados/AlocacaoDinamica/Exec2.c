#include <stdio.h>

int main(){
    int matriz [4][4] = { 
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,10}

    };
    int maior;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
        if (matriz[i][j] > maior){
            maior = matriz[i][j];
        }
        }

    }

           printf("%d \n" ,maior);
}