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
    if (quantityOut == NULL) {
        return -1;
    }

    int product_index = -1;
    for (int i = 0; i < g_product_count; i++) {
        if (g_products[i].is_active && g_products[i].product_id == productId) {
            product_index = i;
            break;
        }
    }

    if (product_index < 0) {
        printf("error: 指定された商品が見つかりません。\n");
        return -1;
    }

    int max_quantity = g_products[product_index].stock;
    int quantity;

    if (max_quantity <= 0) {
        return -2; // 在庫切れ
    }

    if (readInt08("購入数量を入力してください: ", &quantity) != 0) {
        printf("error: 入力を読み取れませんでした。\n");
        return -1;
    }

    if (quantity <= 0) {
        printf("error: 1以上の数量を入力してください。\n");
        return -1;
    }

    if (quantity > max_quantity) {
        printf("error: 在庫を超えています。最大 %d 個まで入力できます。\n", max_quantity);
        return -2;
    }

    *quantityOut = quantity;
    return 0;
}
