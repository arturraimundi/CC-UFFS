#include <stdio.h>

int main(){
    int tN;
    int nRange;
    int n;
    scanf("%d", &n);
    while(n > 0){
        if(n >= 10 & n <= 20){
            nRange = nRange + 1;
        }
        tN = tN + 1;

        scanf("%d", &n);
    }

    printf("%d porcento", 100*nRange/tN);

}