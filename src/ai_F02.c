#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 50

// 商品構造体
typedef struct {
    int id;
    char name[50];
    int price;
    int stock;
} Product;

// 商品登録関数
void registerProduct(Product products[], int *productCount) {
    if (*productCount >= MAX_PRODUCTS) {
        printf("\n商品登録エラー: 商品数が上限に達しています。\n");
        return;
    }

    Product newProduct;
    newProduct.id = *productCount + 1; // 自動的にIDを割り当て

    printf("\n新しい商品を登録します。\n");
    printf("商品名を入力してください: ");
    scanf("%49s", newProduct.name);

    printf("価格を入力してください: ");
    while (scanf("%d", &newProduct.price) != 1 || newProduct.price < 0) {
        printf("無効な入力です。正の整数を入力してください: ");
        while (getchar() != '\n'); // 入力バッファをクリア
    }

    printf("在庫数を入力してください: ");
    while (scanf("%d", &newProduct.stock) != 1 || newProduct.stock < 0) {
        printf("無効な入力です。正の整数を入力してください: ");
        while (getchar() != '\n'); // 入力バッファをクリア
    }

    products[*productCount] = newProduct;
    (*productCount)++;

    printf("\n商品が正常に登録されました。\n");
    printf("ID: %d, 商品名: %s, 価格: %d, 在庫数: %d\n", newProduct.id, newProduct.name, newProduct.price, newProduct.stock);
}

// メイン関数（テスト用）
int main() {
    Product products[MAX_PRODUCTS];
    int productCount = 0; // 現在の登録商品数

    registerProduct(products, &productCount);

    return 0;
}
