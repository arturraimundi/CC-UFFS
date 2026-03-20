#include <stdio.h>

void troca (int *n1, int *n2)
{
	int aux=*n1;
	*n1=*n2;
	*n2=aux;
}

void maior (int i1, int i2, int *p)
{  
	if (i1>i2) {*p=i1;}
	else       {*p=i2;}
}
int main()
{	
   int a=5, b=10, m;
   printf("a=%d b=%d\n",a,b);
   troca(&a,&b);
   printf("a=%d b=%d\n",a,b);
   maior(23,34,&m);
   printf("Maior: %d\n",m);
   return 0;
}   
