#include <stdio.h>
#include "global.c"

int result, ten, fifty, hundred, five_hundred, thousand;
int* return_money[5] = {&ten, &fifty, &hundred, &five_hundred, &thousand};

void refund() {
    
    
    if (total_money==0) {
        return;
    }

    result = total_money;
    // 10円、50円、100円、500円、1000円のカウント
    thousand = result / 1000;
    result = result % 1000;
    five_hundred = result / 500;
    result = result % 500;
    hundred = result / 100;
    result = result % 100;
    fifty = result / 50;
    result = result % 50;
    ten = result / 10;
    result = result % 10;

    printf("おつり: 1000円%d\n", thousand);
    printf("500円%d\n", five_hundred);
    printf("100円%d\n", hundred);
    printf("50円%d\n", fifty);
    printf("10円%d\n", ten);
    
    return;
}

void cancel() {
    printf("残高%d円を返却します。\n", total_money);
    return;
}