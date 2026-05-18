#include <stdio.h>>
#include "global.c"
#include "F01.c"

int total_money=0;

void handle_deposit(){
    int coin_1000=0;
    int coin_500=0;
    int coin_100=0;
    int coin_50=0;
    int coin_10=0;
    int total_coin=0;
    printf("各硬貨の枚数を入力してください。\n");

    printf("1000円の枚数: ");
    if (scanf("%d", &coin_1000)!=1)
    return;

    printf("500円の枚数: ");
    if (scanf("%d", &coin_500)!=1)
    return;

    printf("100円の枚数: ");
    if (scanf("%d", &coin_100)!=1)
    return;

    printf("50円の枚数: ");
    if (scanf("%d", &coin_50)!=1)
    return;

    printf("10円の枚数: ");
    if (scanf("%d", &coin_10)!=1)
    return;

    total_coin = coin_1000 * 1000 + coin_500 * 500 + coin_100 * 100 + coin_50 * 50 + coin_10 * 10;
    total_money += total_coin;

    printf("現在の預り金: %d円\n", total_money);
}