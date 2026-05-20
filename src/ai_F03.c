#include <stdio.h>
#include <string.h>
#include "global.h"

// 商品情報編集関数（global.hのグローバル商品マスタを更新）
void editProduct(void) {
    int product_id;
    printf("\n編集する商品のIDを入力してください: ");
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

    printf("\n商品ID: %d, 現在の商品情報:\n", g_products[index].product_id);
    printf("商品名: %s, 価格: %d, 在庫数: %d\n", g_products[index].product_name, g_products[index].price, g_products[index].stock);

    int choice;
    printf("\n編集する項目を選択してください:\n");
    printf("1. 商品名\n2. 価格\n3. 在庫数\n選択: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        printf("新しい商品名を入力してください: ");
        scanf("%63s", g_products[index].product_name);
        printf("商品名が更新されました。\n");
        break;
    case 2:
        printf("新しい価格を入力してください: ");
        while (scanf("%d", &g_products[index].price) != 1 || g_products[index].price < 0) {
            printf("無効な入力です。0以上の整数を入力してください: ");
            while (getchar() != '\n') {
            }
        }
        printf("価格が更新されました。\n");
        break;
    case 3:
        printf("新しい在庫数を入力してください: ");
        while (scanf("%d", &g_products[index].stock) != 1 ||
               g_products[index].stock < 0 ||
               g_products[index].stock > MAX_STOCK_PER_PRODUCT) {
            printf("無効な入力です。0〜%dの整数を入力してください: ", MAX_STOCK_PER_PRODUCT);
            while (getchar() != '\n') {
            }
        }
        printf("在庫数が更新されました。\n");
        break;
    default:
        printf("無効な選択です。編集を終了します。\n");
        return;
    }

    printf("\n更新後の商品情報:\n");
    printf("商品ID: %d, 商品名: %s, 価格: %d, 在庫数: %d\n",
           g_products[index].product_id,
           g_products[index].product_name,
           g_products[index].price,
           g_products[index].stock);
}

// int main(void) {
//     g_product_count = 3;
//     g_products[0] = (Product){1, "コーラ", 120, 10, 1};
//     g_products[1] = (Product){2, "お茶", 100, 20, 1};
//     g_products[2] = (Product){3, "水", 80, 15, 1};

//     editProduct();
//     return 0;
// }
