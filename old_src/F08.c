#include <stdio.h>
#include <stdbool.h>
#include "global.c"
#include "F01.h"

static int coin_100_stock = 5;
static int coin_50_stock = 5;
static int coin_10_stock = 5;

bool can_make_change(int change, int *out100, int *out50, int *out10) {
    for (int use100 = change / 100; use100 >= 0; use100--) {
        if (use100 > coin_100_stock) continue;
        int remain_after_100 = change - use100 * 100;

        for (int use50 = remain_after_100 / 50; use50 >= 0; use50--) {
            if (use50 > coin_50_stock) continue;
            int remain_after_50 = remain_after_100 - use50 * 50;

            if (remain_after_50 % 10 != 0) continue;
            int use10 = remain_after_50 / 10;
            if (use10 > coin_10_stock) continue;

            *out100 = use100;
            *out50 = use50;
            *out10 = use10;
            return true;
        }
    }
    return false;
}

void dispense_change(int change, int use100, int use50, int use10) {
    coin_100_stock -= use100;
    coin_50_stock -= use50;
    coin_10_stock -= use10;

    printf("おつりは %d 円です。\n", change);
    printf("100円玉: %d 枚\n", use100);
    printf("50円玉: %d 枚\n", use50);
    printf("10円玉: %d 枚\n", use10);
    printf("おつりを出しました。\n");
}

void handle_change_after_purchase(int price) {
    int change = total_money - price;

    if (total_money < price) {
        printf("支払金額が不足しています。購入できません。\n");
        return;
    }
    if (change == 0) {
        printf("ぴったり支払われました。おつりはありません。\n");
        total_money = 0;
        return;
    }

    int use100 = 0, use50 = 0, use10 = 0;
    if (!can_make_change(change, &use100, &use50, &use10)) {
        printf("おつりが用意できません。\n");
        printf("%d円を返金します。\n", total_money);
        total_money = 0;
        return;
    }

    dispense_change(change, use100, use50, use10);
    total_money = 0;
}
