#include <stdio.h>

int main(){

    int n;
    int Tp;
    int Tn;

for(int i = 0; i < 4; i++){
    printf("Numero: ");
    scanf("%d", &n);

    if(n >= 0){
        Tp =Tp  + 1;
    } else {
        Tn = Tn + 1;
    }
}
    printf("%d (+) e %d (-)",  Tp, Tn);
  
}

