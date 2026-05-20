
#include <stdio.h>      // 標準入出力（printfなど）
#include "global.h"    // 商品情報・グローバル変数の定義

// =============================
// F09: 合計金額表示
// =============================
// 商品IDと数量を受け取り、該当商品の単価×数量を計算し、合計金額を表示する関数
// 他の機能（購入処理など）から呼び出して利用します
//
// 引数:
//   product_id : ユーザーが選択した商品ID
//   quantity   : ユーザーが入力した購入数量
// 戻り値:
//   合計金額（小計）。商品IDが不正な場合は-1を返す
int displaySubtotal(int product_id, int quantity) {
    // 1. 商品IDから商品情報を検索
    for (int i = 0; i < g_product_count; i++) {
        // 有効な商品かつID一致
        if (g_products[i].is_active && g_products[i].product_id == product_id) {
            int price = g_products[i].price;           // 単価を取得
            int subtotal = price * quantity;           // 合計金額（小計）を計算

            // 2. 合計金額を画面に表示
            printf("\n【合計金額表示】\n");
            printf("商品: %s\n", g_products[i].product_name);
            printf("単価: %d 円 × 数量: %d = 小計: %d 円\n", price, quantity, subtotal);
            // ※税込や端数処理が必要な場合はここで追加
            printf("合計金額: %d 円\n", subtotal);

            return subtotal;
        }
    }
    // 3. 商品IDが見つからなかった場合のエラー表示
    printf("エラー: 指定された商品IDが見つかりません。\n");
    return -1;
}

// =============================
// テスト用main関数（他機能と結合時は不要）
// =============================
#ifdef TEST_F09_MAIN
int main() {
    // --- テスト用の初期データ設定 ---
    g_product_count = 2;
    g_products[0].product_id = 1;
    snprintf(g_products[0].product_name, MAX_NAME_LEN, "コーラ");
    g_products[0].price = 120;
    g_products[0].stock = 10;
    g_products[0].is_active = 1;

    g_products[1].product_id = 2;
    snprintf(g_products[1].product_name, MAX_NAME_LEN, "お茶");
    g_products[1].price = 100;
    g_products[1].stock = 20;
    g_products[1].is_active = 1;

    // --- 合計金額表示のテスト実行 ---
    displaySubtotal(1, 2); // コーラ2本
    return 0;
}
#endif