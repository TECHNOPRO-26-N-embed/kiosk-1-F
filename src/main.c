#include <stdio.h>
#include "global.c"
#include "F01.c"
#include "F02.c"
#include "F03.c"
// #include "F04.c"
// #include "F05.c"
#include "F06.c"
#include "F07.c"
#include "F01.h"


int main(void) {
    int user_input;
    do {
        printf("=== メニュー ===\n");
        Drink();
        printf("=== 操作 ===\n");
        printf("1: お金を入れる\n");
        printf("2: 飲み物を選ぶ\n");
        printf("3: 在庫を補充する\n");
        printf("8: 購入キャンセル\n");
        printf("9: 終了\n");
        if(scanf("%d", &user_input) != 1 && (user_input < 1 || user_input > 9)) {
            printf("error: 半角数字1~9で入力してください。\n");
            while (getchar() != '\n');
            continue;
        }
        switch(user_input) {
            case 1:
                handle_deposit();
                break;
            case 2:
                select_drink(data);
                refund();
                break;
            case 3:
                update_stock();
                break;
            case 8:
                cancel();
                break;
            case 9:
                printf("終了します。\n");
                break;
            default:
                printf("無効な選択です。もう一度入力してください。\n");
                break;
        }
    } while (user_input != 9);
    return 0;
}
