#include <stdio.h>
#include "global.c"
#include "F01.c"
#include "F02.c"
#include "F03.c"
// #include "F04.c"
// #include "F05.c"
#include "F06.c"
#include "F07.c"


int main(void) {
    int user_input;
    do {
        printf("=== メニュー ===\n");
        printf("1: お金を入れる\n");
        printf("9: 終了\n");
        if(scanf("%d", &user_input) != 1){
            printf("error: 半角数字で入力してください。\n");            
            while (getchar() != '\n');
            continue;
        }
    } while (user_input != 9);
    return 0;
}
