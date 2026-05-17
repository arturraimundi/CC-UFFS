#include <stdio.h>
#include "data.h"

void setDate(data *d, int dia, int mes, int ano) {
    d->dia = dia;
    d->mes = mes;
    d->ano = ano;
}

int getDate(data d, char tipo) {
    if (tipo == 'D')
        return d.dia;

    if (tipo == 'M')
        return d.mes;

    if (tipo == 'Y')
        return d.ano;

    return -1;
}

void showDate(data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

int dataComp(data d1, data d2) {

    if (d1.ano > d2.ano)
        return 1;

    if (d1.ano < d2.ano)
        return -1;

    if (d1.mes > d2.mes)
        return 1;

    if (d1.mes < d2.mes)
        return -1;

    if (d1.dia > d2.dia)
        return 1;

    if (d1.dia < d2.dia)
        return -1;

    return 0;
}

int isLeapYear(data d) {

    if ((d.ano % 4 == 0 && d.ano % 100 != 0) || (d.ano % 400 == 0))
        return 1;

    return 0;
}

int getDayFromYear(data d) {

    int diasMes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int soma = 0;
    int i;

    if (isLeapYear(d))
        diasMes[1] = 29;

    for (i = 0; i < d.mes - 1; i++) {
        soma = soma + diasMes[i];
    }

    soma = soma + d.dia;

    return soma;
}

int totalDias(data d) {

    int total = 0;
    int ano;

    for (ano = 1; ano < d.ano; ano++) {

        if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
            total = total + 366;
        else
            total = total + 365;
    }

    total = total + getDayFromYear(d);

    return total;
}

int getDiff(data d1, data d2) {

    int t1 = totalDias(d1);
    int t2 = totalDias(d2);

    if (t1 > t2)
        return t1 - t2;
    else
        return t2 - t1;
}