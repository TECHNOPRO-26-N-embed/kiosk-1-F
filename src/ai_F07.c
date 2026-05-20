#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "ai_F14.h"

// 商品選択および数量入力を担当するモジュール
// - 商品一覧を表示し、ユーザーが購入する商品を選択できる
// - 選択された商品の数量を入力し、在庫を確認する

int displaySubtotal(int product_id, int quantity);
int receivePayment(int subtotal, int *amountReceivedOut);
int calculateChange(int inserted_amount, int subtotal);
int finalizeTransaction(Transaction *trans);
int inputQuantity(int productId, int *quantityOut);

static void flushInputBuffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

static int readInt(const char *prompt, int *outValue)
{
    int value;

    if (outValue == NULL) {
        return -1;
    }

    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            flushInputBuffer();
            *outValue = value;
            return 0;
        }

        if (feof(stdin)) {
            return -1;
        }

        printf("error: 半角数字で入力してください。\n");
        flushInputBuffer();
    }
}

static int findProductIndex(int product_id)
{
    for (int i = 0; i < g_product_count; i++) {
        if (g_products[i].is_active && g_products[i].product_id == product_id) {
            return i;
        }
    }
    return -1;
}

// 商品一覧を表示する関数
// - 在庫がある商品のみを表示
// - 商品ID、名前、価格、在庫数をフォーマットして出力
static void printAvailableProducts(void)
{
    printf("\n=== 商品選択 ===\n");
    printf("%-5s %-20s %-8s %-8s\n", "ID", "商品名", "価格", "在庫");
    printf("---------------------------------------------\n");

    for (int i = 0; i < g_product_count; i++) {
        if (!g_products[i].is_active) {
            continue;
        }

        if (g_products[i].stock <= 0) {
            printf("%-5d %-20s %-8d %-8s\n",
                   g_products[i].product_id,
                   g_products[i].product_name,
                   g_products[i].price,
                   "在庫0");
        } else {
            printf("%-5d %-20s %-8d %-8d\n",
                   g_products[i].product_id,
                   g_products[i].product_name,
                   g_products[i].price,
                   g_products[i].stock);
        }
    }
}

// 商品を選択する関数
// - ユーザーが入力した商品IDを検証
// - 在庫がある場合に選択を確定
static int selectProduct(void)
{
    int product_id;
    int product_index;

    while (1) {
        printAvailableProducts();
        if (readInt("購入する商品IDを入力してください: ", &product_id) != 0) {
            printf("error: 入力を読み取れませんでした。\n");
            return -1;
        }

        product_index = findProductIndex(product_id);
        if (product_index < 0) {
            printf("error: 存在しない商品IDです。もう一度入力してください。\n");
            continue;
        }

        if (g_products[product_index].stock <= 0) {
            printf("error: 選択された商品は在庫切れです。別の商品を選択してください。\n");
            continue;
        }

        g_current_transaction.product_id = g_products[product_index].product_id;
        g_current_transaction.unit_price = g_products[product_index].price;
        printf("選択された商品: %s (%d 円)\n",
               g_products[product_index].product_name,
               g_products[product_index].price);
        return 0;
    }
}

/* inputQuantity は ai_F08.c に実装されています */

void initializeSystem(void)
{
    // CSVファイルの初期化のみ（表示なし）
    if (initializeLogFiles() != 0) {
        fprintf(stderr, "warning: ログファイルの初期化に失敗しました。\n");
    }

    if (g_product_count == 0) {
        g_product_count = 50;
        for (int i = 0; i < 50; i++) {
            g_products[i].product_id = i + 1;
            snprintf(g_products[i].product_name, MAX_NAME_LEN, "商品%d", i + 1);
            g_products[i].price = 100 + (i + 1) * 10;
            g_products[i].stock = 50;
            g_products[i].is_active = 1;
        }
    }
}

int executePurchase(void)
{
    int subtotal;
    int inserted_amount;
    int change;

    memset(&g_current_transaction, 0, sizeof(g_current_transaction));
    g_current_transaction.session_id = g_current_session_id;
    g_current_transaction.transaction_id = g_next_transaction_id;

    /* 商品選択→数量入力省略。常に1個購入 */
    while (1) {
        if (selectProduct() != 0) {
            return -1;
        }
        int product_index = findProductIndex(g_current_transaction.product_id);
        if (product_index < 0) {
            printf("error: 商品が見つかりません。\n");
            return -1;
        }
        if (g_products[product_index].stock < 1) {
            printf("error: 在庫不足です。別の商品を選択してください。\n");
            continue;
        }
        g_current_transaction.quantity = 1;
        break;
    }

    subtotal = displaySubtotal(g_current_transaction.product_id, g_current_transaction.quantity);
    if (subtotal < 0) {
        handleError("PURCHASE_ERROR", "displaySubtotal", "", "小計の計算に失敗しました。", "MEDIUM");
        return -1;
    }
    g_current_transaction.subtotal = subtotal;

    if (receivePayment(subtotal, &inserted_amount) != 0) {
        handleError("PAYMENT_ERROR", "receivePayment", "", "現金投入に失敗しました。", "MEDIUM");
        return -1;
    }
    g_current_transaction.inserted_amount = inserted_amount;

    change = calculateChange(inserted_amount, subtotal);
    if (change < 0) {
        handleError("PAYMENT_ERROR", "calculateChange", "", "釣銭計算に失敗しました。", "MEDIUM");
        return -1;
    }
    g_current_transaction.change_amount = change;

    if (change > 0) {
        printf("\n[案内] 釣銭: %d 円を返却します。\n", change);
    } else {
        printf("\n[案内] 釣銭はありません。\n");
    }

    if (finalizeTransaction(&g_current_transaction) != 0) {
        handleError("PURCHASE_ERROR", "finalizeTransaction", "", "購入確定処理に失敗しました。", "HIGH");
        return -1;
    }

    if (logSale(&g_current_transaction, "OK") != 0) {
        handleError("LOG_ERROR", "logSale", "", "販売ログの記録に失敗しました。", "LOW");
    }

    if (logOperation("BUY", "purchase completed", "OK", "") != 0) {
        handleError("LOG_ERROR", "logOperation", "", "操作ログの記録に失敗しました。", "LOW");
    }

    g_next_transaction_id++;
    printf("\n購入が完了しました。ありがとうございました。\n");
    return 0;
}
