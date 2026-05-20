#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ai_main.c
 *
 * 役割:
 * - 他ファイルに分割実装される関数を呼び出すためのエントリ/ハブ
 * - 実装ファイル名が未確定でも使えるようにextern宣言で接続
 *
 * 後でヘッダが確定したら、extern宣言を削除して #include "xxx.h" に置き換えてください。
 */

#define MAX_SESSION_LEN 20
#define MAX_NAME_LEN 100

/* ===== 型定義（将来は共通ヘッダへ移動推奨） ===== */
typedef struct {
	int transactionId;
	char sessionId[MAX_SESSION_LEN];
	int productId;
	char productName[MAX_NAME_LEN];
	int price;
	int quantity;
	int subtotal;
	int amountReceived;
	int change;
	char timestamp[20];
	char result[10];
} Transaction;

typedef struct {
	int logId;
	char sessionId[MAX_SESSION_LEN];
	char timestamp[20];
	char operationType[30];
	char details[200];
	char result[10];
	int errorCode;
} OperationLog;

/* ===== 外部関数宣言（他ファイル実装予定） ===== */
extern void initializeSystem(void);
extern int displayMainMenu(void);
extern int getValidInput(int min, int max);
extern char *getInputString(int maxLen);
extern void getCurrentTimestamp(char *buf);

extern int displayProductList(void);
extern int registerProduct(void);
extern int editProduct(void);
extern int deleteProduct(void);
extern int executePurchase(void);

extern int selectProduct(void);
extern int inputQuantity(int productId, int *quantityOut);
extern int calculateSubtotal(int price, int quantity);
extern int receivePayment(int subtotal, int *amountReceivedOut);
extern int calculateChange(int amountReceived, int subtotal);
extern int finalizeTransaction(Transaction *trans);

extern int appendOperationLog(OperationLog *log);
extern int appendTransactionLog(Transaction *trans);
extern int appendFailurePredictLog(const char *errorType,
								   const char *location,
								   const char *inputValue,
								   const char *message,
								   int severity);

static int handleMainMenuSelection(int selectedMenu)
{
	switch (selectedMenu) {
	case 1:
		return displayProductList();
	case 2:
		return registerProduct();
	case 3:
		return editProduct();
	case 4:
		return deleteProduct();
	case 5:
		return executePurchase();
	case 6:
		/* TODO: 必要ならログ閲覧機能を別関数として追加 */
		printf("[INFO] ログ機能は実装予定です。\n");
		return 0;
	case 9:
		return 1; /* 終了シグナル */
	default:
		printf("[WARN] 無効な選択です。\n");
		return 0;
	}
}

/*
 * 他ファイルから呼び出せる実行関数。
 * 例: main.c から runKioskApp();
 */
int runKioskApp(void)
{
	int selectedMenu;
	int shouldExit = 0;

	initializeSystem();

	while (!shouldExit) {
		selectedMenu = displayMainMenu();
		if (handleMainMenuSelection(selectedMenu) == 1) {
			shouldExit = 1;
		}

		/*
		 * F13: 複数客対応（連続利用）
		 * 購入完了後もシステムは終了せず、次のお客様が続けて利用可能。
		 */
		if (!shouldExit) {
			printf("\n[案内] 次のお客様どうぞ。\n\n");
		}
	}

	printf("[INFO] システムを終了します。\n");
	return 0;
}

/*
 * 各関数の使用例テンプレート。
 * 実行はしません。必要に応じて呼び出して利用してください。
 */
void apiUsageExamples(void)
{
	int menu;
	int productId;
	int quantity;
	int subtotal;
	int amountReceived;
	int change;
	char *nameBuf;
	char timestamp[20] = {0};
	Transaction trans;
	OperationLog opLog;

	memset(&trans, 0, sizeof(trans));
	memset(&opLog, 0, sizeof(opLog));

	initializeSystem();
	menu = displayMainMenu();
	(void)menu;

	(void)getValidInput(1, 9);
	nameBuf = getInputString(99);
	free(nameBuf);

	getCurrentTimestamp(timestamp);

	(void)displayProductList();
	(void)registerProduct();
	(void)editProduct();
	(void)deleteProduct();
	(void)executePurchase();

	productId = selectProduct();
	(void)inputQuantity(productId, &quantity);
	subtotal = calculateSubtotal(120, 2);
	(void)receivePayment(subtotal, &amountReceived);
	change = calculateChange(amountReceived, subtotal);
	(void)change;

	(void)finalizeTransaction(&trans);
	(void)appendOperationLog(&opLog);
	(void)appendTransactionLog(&trans);
	(void)appendFailurePredictLog("INPUT_ERROR",
								  "displayMainMenu",
								  "abc",
								  "数字以外が入力されました",
								  2);
}

