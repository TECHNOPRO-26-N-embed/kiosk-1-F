/*
 * ai_main.c
 * 役割:
 * - デジタル自販機システムのエントリーポイント
 * - メニューを表示し、ユーザーの選択に応じて各機能を呼び出す
 * - システム全体の初期化と終了処理を管理
 */

#include <stdio.h>
#include "global.h"

/* 関数プロトタイプ（各ai_Fxx.cで実装） */
// 商品一覧を表示する関数（F01）
void displayProductList(void);
// システムを初期化する関数（F07）
void initializeSystem(void);
// 購入処理を実行する関数（F12）
int executePurchase(void);
// ログを表示しCSVに保存する関数（F14）
int showAndSaveLogs(void);
// 商品を補充する関数（F06）
void restockProducts(void);

/*
 * 標準入力バッファをクリアする関数
 * - 入力エラー時にバッファをクリアして再入力を促す
 */
static void flushInputBufferMain(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

/*
 * メイン関数
 * - 自販機システムのエントリーポイント
 * - メニューを表示し、ユーザーの選択に応じて各機能を呼び出す
 */
int main(void) {
    int userInput = 0;

    // システム初期化
    initializeSystem();

    do {
        // メニュー表示
        printf("\n=== デジタル自販機メニュー ===\n");
        printf("1: 商品一覧表示\n");
        printf("2: 購入処理\n");
        printf("3: ログ表示/CSV保存\n");
        printf("4: 商品の補充\n"); // 新しいメニュー項目を追加
        printf("9: 終了\n");
        printf("選択してください (1-4, 9): ");

        // ユーザー入力を取得
        if (scanf("%d", &userInput) != 1) {
            printf("error: 半角数字で入力してください。\n");
            flushInputBufferMain();
            continue;
        }
        flushInputBufferMain();

        // ユーザー選択に応じた処理
        switch (userInput) {
        case 1:
            // 商品一覧表示
            displayProductList();
            break;
        case 2:
            // 購入処理
            if (executePurchase() != 0) {
                printf("購入処理が完了しませんでした。\n");
            }
            break;
        case 3:
            // ログ表示とCSV保存
            showAndSaveLogs();
            break;
        case 4:
            // 商品補充
            restockProducts();
            break;
        case 9:
            // 終了
            printf("終了します。\n");
            break;
        default:
            // 無効な入力
            printf("error: 1〜4 または 9 を入力してください。\n");
            break;
        }
    } while (userInput != 9);

    return 0;
}

