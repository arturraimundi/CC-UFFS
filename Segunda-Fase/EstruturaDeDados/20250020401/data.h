#ifndef DATA_H
#define DATA_H

typedef struct {
    int dia;
    int mes;
    int ano;
} data;

void setDate(data *d, int dia, int mes, int ano);
int getDate(data d, char tipo);
void showDate(data d);
int dataComp(data d1, data d2);
int getDiff(data d1, data d2);
int isLeapYear(data d);
int getDayFromYear(data d);

#endif