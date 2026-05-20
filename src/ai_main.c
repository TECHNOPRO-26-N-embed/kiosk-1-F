#include <stdio.h>
#include "global.h"
#include "ai_F01.c"
#include "ai_F11.c"
#include "ai_F12.c"
#include "ai_F14.c"

/* ===== スタブ関数（未実装部分） ===== */
int selectProduct(void) {
    printf("[スタブ] 商品選択を実行中...\n");
    return 1; // 仮に商品ID 1 を返す
}

int inputQuantity(int productId, int *quantityOut) {
    printf("[スタブ] 数量入力を実行中...\n");
    *quantityOut = 1; // 仮に数量 1 を設定
    return 0; // 正常終了
}

/* ===== メイン関数 ===== */
int main(void) {
    int userInput = 0;
    int productId;
    int quantity;
    int subtotal;
    int insertedAmount;
    int change;
    Transaction trans = {0};

    initializeSystem();

    do {
        printf("\n=== デジタル自販機メニュー ===\n");
        printf("1: 商品一覧表示\n");
        printf("2: 購入処理\n");
        printf("3: ログ表示/CSV保存\n");
        printf("9: 終了\n");
        printf("選択してください (1-3, 9): ");

        if (scanf("%d", &userInput) != 1) {
            printf("error: 半角数字で入力してください。\n");
            continue;
        }

        switch (userInput) {
        case 1:
            displayProductList();
            break;
        case 2:
            // 商品選択（F07）
            productId = selectProduct();
            if (productId < 0) {
                printf("商品選択がキャンセルされました。\n");
                break;
            }

            // 数量入力（F08）
            if (inputQuantity(productId, &quantity) != 0) {
                printf("数量入力に失敗しました。\n");
                break;
            }

            // 釣銭計算（F11）
            subtotal = quantity * 100; // 仮に単価100円とする
            printf("小計: %d 円\n", subtotal);
            insertedAmount = 500; // 仮に500円投入
            change = calculateChange(insertedAmount, subtotal);
            if (change < 0) {
                printf("投入金額が不足しています。\n");
                break;
            }
            printChange(change);

            // 購入確定（F12）
            trans.transaction_id = 1; // 仮の取引ID
            trans.product_id = productId;
            trans.quantity = quantity;
            trans.unit_price = 100; // 仮の単価
            trans.subtotal = subtotal;
            trans.inserted_amount = insertedAmount;
            trans.change_amount = change;

            if (finalizeTransaction(&trans) != 0) {
                printf("購入確定に失敗しました。\n");
                break;
            }

            printf("購入が正常に完了しました。\n");
            break;
        case 3:
            showAndSaveLogs();
            break;
        case 9:
            printf("終了します。\n");
            break;
        default:
            printf("error: 1〜3 または 9 を入力してください。\n");
            break;
        }
    } while (userInput != 9);

    return 0;
}

