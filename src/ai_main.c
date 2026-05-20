#include <stdio.h>
#include "global.h"

/* 関数プロトタイプ（各ai_Fxx.cで実装） */
void displayProductList(void);
void initializeSystem(void);
int executePurchase(void);
int showAndSaveLogs(void);
void restockProducts(void); // プロトタイプ宣言を追加
void editProduct(void); // プロトタイプ宣言を追加
void registerProduct(void); // プロトタイプ宣言を追加

static void flushInputBufferMain(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

/* ===== メイン関数 ===== */
int main(void) {
    int userInput = 0;

    initializeSystem();

    do {
        printf("\n=== デジタル自販機メニュー ===\n");
        printf("1: 商品一覧表示\n");
        printf("2: 購入処理\n");
        printf("3: ログ表示/CSV保存\n");
        printf("4: 商品の補充\n");
        printf("5: 商品登録\n");
        printf("6: 商品情報編集\n");
        printf("9: 終了\n");
        printf("選択してください (1-6, 9): ");

        if (scanf("%d", &userInput) != 1) {
            printf("error: 半角数字で入力してください。\n");
            flushInputBufferMain();
            continue;
        }
        flushInputBufferMain();

        switch (userInput) {
        case 1:
            displayProductList();
            break;
        case 2:
            if (executePurchase() != 0) {
                printf("購入処理が完了しませんでした。\n");
            }
            break;
        case 3:
            showAndSaveLogs();
            break;
        case 4:
            restockProducts();
            break;
        case 5:
            registerProduct(); // 商品登録を呼び出し
            break;
        case 6:
            editProduct(); // 商品情報編集を呼び出し
            break;
        case 9:
            printf("終了します。\n");
            break;
        default:
            printf("error: 1〜6 または 9 を入力してください。\n");
            break;
        }
    } while (userInput != 9);

    return 0;
}

