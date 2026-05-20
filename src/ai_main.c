#include <stdio.h>
#include "global.h"

/*
 * ai_main.c
 * 役割:
 * - ユーザーが直接操作するエントリポイント
 * - 機能本体は他ファイルの関数を呼び出して実行
 *
 * いまはファイル名未確定のため extern で宣言しています。
 * 後でヘッダが確定したら #include に置き換えてください。
 */

/* ===== 他ファイル実装予定の関数 ===== */
extern void initializeSystem(void);
extern int displayProductList(void);    /* F01 */
extern int registerProduct(void);       /* F02 */
extern int editProduct(void);           /* F03 */
extern int deleteProduct(void);         /* F04 */
extern int executePurchase(void);       /* F05-F12 */
extern int showAndSaveLogs(void);       /* F14: 未実装なら-1などを返す */

static void flushInputBuffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

static int readMenuInput(int *outValue)
{
    if (scanf("%d", outValue) != 1) {
        flushInputBuffer();
        return -1;
    }
    flushInputBuffer();
    return 0;
}

static void printMainMenu(void)
{
    printf("\n=== デジタル自販機メニュー ===\n");
    printf("1: 商品一覧表示\n");
    printf("2: 商品登録\n");
    printf("3: 商品情報編集\n");
    printf("4: 商品削除\n");
    printf("5: 購入処理\n");
    printf("6: ログ表示/CSV保存\n");
    printf("9: 終了\n");
    printf("選択してください (1-6, 9): ");
}

int main(void)
{
    int userInput = 0;
    int purchaseResult;

    initializeSystem();

    if (g_current_session_id <= 0) {
        g_current_session_id = 1;
    }

    do {
        printMainMenu();

        if (readMenuInput(&userInput) != 0) {
            printf("error: 半角数字で入力してください。\n");
            continue;
        }

        switch (userInput) {
        case 1:
            (void)displayProductList();
            break;
        case 2:
            (void)registerProduct();
            break;
        case 3:
            (void)editProduct();
            break;
        case 4:
            (void)deleteProduct();
            break;
        case 5:
            purchaseResult = executePurchase();
            if (purchaseResult == 0) {
                g_current_session_id++;
                printf("\n[案内] ご購入ありがとうございました。次のお客様どうぞ。\n");
            }
            break;
        case 6:
            (void)showAndSaveLogs();
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

