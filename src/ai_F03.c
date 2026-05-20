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

// 商品情報編集関数
void editProduct(Product products[], int productCount) {
    int productId;
    printf("\n編集する商品のIDを入力してください: ");
    scanf("%d", &productId);

    // 商品IDの確認
    int index = -1;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == productId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nエラー: 指定された商品IDが見つかりません。\n");
        return;
    }

    printf("\n商品ID: %d, 現在の商品情報:\n", products[index].id);
    printf("商品名: %s, 価格: %d, 在庫数: %d\n", products[index].name, products[index].price, products[index].stock);

    int choice;
    printf("\n編集する項目を選択してください:\n");
    printf("1. 商品名\n2. 価格\n3. 在庫数\n選択: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("新しい商品名を入力してください: ");
            scanf("%49s", products[index].name);
            printf("商品名が更新されました。\n");
            break;
        case 2:
            printf("新しい価格を入力してください: ");
            while (scanf("%d", &products[index].price) != 1 || products[index].price < 0) {
                printf("無効な入力です。正の整数を入力してください: ");
                while (getchar() != '\n'); // 入力バッファをクリア
            }
            printf("価格が更新されました。\n");
            break;
        case 3:
            printf("新しい在庫数を入力してください: ");
            while (scanf("%d", &products[index].stock) != 1 || products[index].stock < 0) {
                printf("無効な入力です。正の整数を入力してください: ");
                while (getchar() != '\n'); // 入力バッファをクリア
            }
            printf("在庫数が更新されました。\n");
            break;
        default:
            printf("無効な選択です。編集を終了します。\n");
            return;
    }

    printf("\n更新後の商品情報:\n");
    printf("商品ID: %d, 商品名: %s, 価格: %d, 在庫数: %d\n", products[index].id, products[index].name, products[index].price, products[index].stock);
}

// メイン関数（テスト用）
int main() {
    Product products[MAX_PRODUCTS] = {
        {1, "コーラ", 120, 10},
        {2, "お茶", 100, 20},
        {3, "水", 80, 15}
    };

    int productCount = 3; // 現在の登録商品数

    editProduct(products, productCount);

    return 0;
}