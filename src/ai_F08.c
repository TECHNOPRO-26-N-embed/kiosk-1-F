#include <stdio.h>
#include <stdlib.h>
#include "global.h"

static void flushInputBuffer08(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

static int readInt08(const char *prompt, int *outValue)
{
    int value;

    if (outValue == NULL) {
        return -1;
    }

    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            flushInputBuffer08();
            *outValue = value;
            return 0;
        }

        if (feof(stdin)) {
            return -1;
        }

        printf("error: 半角数字で入力してください。\n");
        flushInputBuffer08();
    }
}

// F08: 数量入力
// 引数: productId (商品ID), quantityOut (出力先ポインタ)
// 戻り値: 0 成功, -1 キャンセル/入力エラー, -2 在庫不足
int inputQuantity(int productId, int *quantityOut)
{
    // 数量入力は行わず、常に1個購入
    if (quantityOut == NULL) {
        return -1;
    }
    *quantityOut = 1;
    return 0;
}
