#include <stdio.h>
#include <string.h>
#include "global.h"

// 商品登録関数（global.hのグローバル変数を更新）
void registerProduct(void) {
    if (g_product_count >= MAX_PRODUCTS) {
        printf("\n商品登録エラー: 商品数が上限に達しています。\n");
        return;
    }

    Product new_product = {0};
    new_product.product_id = g_product_count + 1;
    new_product.is_active = 1;

    printf("\n新しい商品を登録します。\n");
    printf("商品名を入力してください: ");
    scanf("%63s", new_product.product_name);

    printf("価格を入力してください: ");
    while (scanf("%d", &new_product.price) != 1 || new_product.price < 0) {
        printf("無効な入力です。0以上の整数を入力してください: ");
        while (getchar() != '\n') {
        }
    }

    printf("在庫数を入力してください: ");
    while (scanf("%d", &new_product.stock) != 1 || new_product.stock < 0 || new_product.stock > MAX_STOCK_PER_PRODUCT) {
        printf("無効な入力です。0〜%dの整数を入力してください: ", MAX_STOCK_PER_PRODUCT);
        while (getchar() != '\n') {
        }
    }

    g_products[g_product_count] = new_product;
    g_product_count++;

    printf("\n商品が正常に登録されました。\n");
    printf("ID: %d, 商品名: %s, 価格: %d, 在庫数: %d\n",
           new_product.product_id, new_product.product_name, new_product.price, new_product.stock);
}

// int main(void) {
//     registerProduct();
//     return 0;
// }
