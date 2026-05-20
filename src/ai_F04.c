#include <stdio.h>
#include "global.h"

// 商品削除関数（配列詰めではなくis_activeを0にして無効化）
void deleteProduct(void) {
    int product_id;
    printf("\n削除する商品のIDを入力してください: ");
    scanf("%d", &product_id);

    int index = -1;
    for (int i = 0; i < g_product_count; i++) {
        if (g_products[i].product_id == product_id && g_products[i].is_active) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nエラー: 指定された商品IDが見つかりません。\n");
        return;
    }

    printf("\n商品ID: %d, 商品名: %s を削除しますか？ (1: はい, 0: いいえ): ",
           g_products[index].product_id,
           g_products[index].product_name);
    int confirm;
    scanf("%d", &confirm);

    if (confirm != 1) {
        printf("\n削除をキャンセルしました。\n");
        return;
    }

    g_products[index].is_active = 0;
    printf("\n商品が正常に削除されました（無効化）。\n");
}

int main(void) {
    g_product_count = 3;
    g_products[0] = (Product){1, "コーラ", 120, 10, 1};
    g_products[1] = (Product){2, "お茶", 100, 20, 1};
    g_products[2] = (Product){3, "水", 80, 15, 1};

    deleteProduct();

    printf("\n現在の商品一覧（有効商品のみ表示）:\n");
    for (int i = 0; i < g_product_count; i++) {
        if (g_products[i].is_active) {
            printf("ID: %d, 商品名: %s, 価格: %d, 在庫数: %d\n",
                   g_products[i].product_id,
                   g_products[i].product_name,
                   g_products[i].price,
                   g_products[i].stock);
        }
    }

    return 0;
}