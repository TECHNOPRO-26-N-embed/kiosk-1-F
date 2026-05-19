#include <stdio.h>

int result, ten, fifty, hundred, five_hundred, thousand;
int* return_money[5] = {&ten, &fifty, &hundred, &five_hundred, &thousand};

int** refund(unsigned int money, unsigned int price) {
    
    
    if (money < price) {
        return 0;
    }
    result = money - price;
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

    return return_money;
}

unsigned int cancel(unsigned int money) {
    return money;
}