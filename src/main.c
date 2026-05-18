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
    printf("=== メニュー ===\n");
    drink();
    printf("1: お金を入れる\n");
    printf("9: 終了\n");
    return 0;
}
