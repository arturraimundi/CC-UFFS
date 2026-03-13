#include <stdio.h>

int main(){

    int max;
    int total;
    scanf("%d", &max);

    for(int i = 0; i <= max; i++){
       total = i + total;
    }
    printf("%d \n", total);
}