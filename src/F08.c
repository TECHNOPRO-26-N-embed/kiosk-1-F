#include <stdio.h>

int main(void) {
    int price = 120;
    int pay = 500;
    int change;

    int coin_100 = 5;
    int coin_50 = 5;
    int coin_10 = 5;

    int n100, n50, n10;
    change = pay - price;

    n100 = change / 100;
    change %= 100;

    n50 = change / 50;
    change %= 50;

    n10 = change / 10;

    if (n100 > coin_100 || n50 > coin_50 || n10 > coin_10) 
    {
        printf("お釣りが足りません。\n");
    } else {
        printf("お釣りは%d円です。\n", pay - price);
        printf("100円玉: %d枚\n", n100);
        printf("50円玉: %d枚\n", n50);
        printf("10円玉: %d枚\n", n10);

    }

    return 0;
}
