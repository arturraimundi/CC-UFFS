#include <stdio.h>
#include "data.h"

int main() {

    data d1, d2;

    setDate(&d1, 10, 10, 2026);
    setDate(&d2, 1, 11, 2026);

    showDate(d1);

    printf("Ano: %d\n", getDate(d2, 'Y'));

    printf("Diferenca: %d dias\n", getDiff(d1, d2));

    return 0;
}