#include <stdio.h>

/* scanf失敗時に入力バッファを破棄する補助関数 */
static void flushInputBuffer(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {
		;
	}
}

/*
 * F10: 現金投入受付
 * 引数:
 *   subtotal          : 支払うべき合計金額
 *   amountReceivedOut : 受け取った投入合計金額の出力先
 * 戻り値:
 *   0  : 充足（投入合計 >= subtotal）
 *  -1  : 入力ストリーム終了(EOF)
 *  -2  : 引数不正
 */
int receivePayment(int subtotal, int *amountReceivedOut)
{
	int inputAmount = 0;
	int totalInserted = 0;

	if (subtotal <= 0 || amountReceivedOut == NULL) {
		printf("error: 合計金額または出力先が不正です。\n");
		return -2;
	}

	printf("\n【現金投入受付】\n");
	printf("お支払い金額: %d 円\n", subtotal);

	/* 合計金額に達するまで追加入金を受け付ける */
	while (totalInserted < subtotal) {
		printf("投入金額を入力してください（残り %d 円）: ", subtotal - totalInserted);

		if (scanf("%d", &inputAmount) != 1) {
			if (feof(stdin)) {
				printf("error: 入力が終了しました。\n");
				return -1;
			}
			printf("error: 半角数字で入力してください。\n");
			flushInputBuffer();
			continue;
		}
		flushInputBuffer();

		if (inputAmount <= 0) {
			printf("error: 1円以上を入力してください。\n");
			continue;
		}

		totalInserted += inputAmount;
		printf("現在の投入合計: %d 円\n", totalInserted);

		if (totalInserted < subtotal) {
			printf("不足金額: %d 円（再投入してください）\n", subtotal - totalInserted);
		}
	}

	*amountReceivedOut = totalInserted;
	printf("投入受付完了: %d 円\n", totalInserted);
	return 0;
}

// #ifdef TEST_F10_MAIN
// int main(void)
// {
// 	int amountReceived = 0;
// 	int result = receivePayment(240, &amountReceived);

// 	if (result == 0) {
// 		printf("最終投入金額: %d 円\n", amountReceived);
// 	}

// 	return 0;
// }
// #endif
