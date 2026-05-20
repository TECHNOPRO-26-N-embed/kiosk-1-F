#include <stdio.h>
#include "global.h"

// 商品入れ替え関数（指定商品IDの情報を新しい内容に更新）
void replaceProduct(void) {
    int product_id;
    printf("\n入れ替える商品のIDを入力してください: ");
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
    printf("商品名: %s, 価格: %d, 在庫数: %d\n",
           g_products[index].product_name,
           g_products[index].price,
           g_products[index].stock);

    printf("\n新しい商品情報を入力してください。\n");

    printf("新しい商品名を入力してください: ");
    scanf("%63s", g_products[index].product_name);

    printf("新しい価格を入力してください: ");
    while (scanf("%d", &g_products[index].price) != 1 || g_products[index].price < 0) {
        printf("無効な入力です。0以上の整数を入力してください: ");
        while (getchar() != '\n') {
        }
    }

    printf("新しい在庫数を入力してください: ");
    while (scanf("%d", &g_products[index].stock) != 1 ||
           g_products[index].stock < 0 ||
           g_products[index].stock > MAX_STOCK_PER_PRODUCT) {
        printf("無効な入力です。0〜%dの整数を入力してください: ", MAX_STOCK_PER_PRODUCT);
        while (getchar() != '\n') {
        }
    }

    printf("\n商品が正常に入れ替えられました。\n");
    printf("更新後の商品情報:\n");
    printf("商品ID: %d, 商品名: %s, 価格: %d, 在庫数: %d\n",
           g_products[index].product_id,
           g_products[index].product_name,
           g_products[index].price,
           g_products[index].stock);
}

// 商品補充関数（全商品の在庫数を最大値に更新）
void restockProducts(void) {
    if (g_product_count == 0) {
        printf("\nエラー: 補充する商品が登録されていません。\n");
        return;
    }

    for (int i = 0; i < g_product_count; i++) {
        if (g_products[i].is_active) {
            g_products[i].stock = MAX_STOCK_PER_PRODUCT;
        }
    }

    printf("\n全ての商品が最大在庫数に補充されました。\n");
}

// int main(void) {
//     g_product_count = 3;
//     g_products[0] = (Product){1, "コーラ", 120, 10, 1};
//     g_products[1] = (Product){2, "お茶", 100, 20, 1};
//     g_products[2] = (Product){3, "水", 80, 15, 1};

//     replaceProduct();
//     restockProducts();
//     return 0;
// }