#include <stdio.h>
#include "global.c"

int result, ten, fifty, hundred, five_hundred, thousand;
int return_money[5] = {1000, 500, 100, 50, 10};

void refund() {
    
    
    if (total_money==0) {
        printf("預り金はありません。\n");
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

    printf("おつり:合計%d円\n", total_money);
    printf("====おつりの内訳====:\n");
    printf("1000円%d\n", thousand);
    printf("500円%d\n", five_hundred);
    printf("100円%d\n", hundred);
    printf("50円%d\n", fifty);
    printf("10円%d\n", ten);

    total_money = 0; // 預り金をリセット

    printf("お釣りを返却しました。終了します。\n");
    
    return;
}

void cancel() {
    if(total_money == 0) {
        printf("預り金はありません。\n");
        return;
    }
    printf("残高%d円を返却します。\n", total_money);
    total_money = 0; // 預り金をリセット
    return;
}