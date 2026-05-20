#include <stdio.h>
#include <string.h>
#include "global.h"

// このファイルは「商品マスタ管理」の例です。
// グローバル変数g_products, g_product_countを軸に、全体の状態を一元管理します。
// 直接配列を操作するのではなく、専用関数経由でアクセスすることで安全性・保守性を高めます。


// 商品一覧を表示する（グローバル変数を参照）
void displayProductList(void) {
    printf("\n商品一覧:\n");
    printf("%-5s %-20s %-10s %-10s\n", "ID", "商品名", "価格", "在庫数");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < g_product_count; i++) {
        // is_active==1 の商品のみ表示
        if (g_products[i].is_active) {
            printf("%-5d %-20s %-10d %-10d\n", g_products[i].product_id, g_products[i].product_name, g_products[i].price, g_products[i].stock);
        }
    }

    printf("\n任意のキーを押してメインメニューに戻ります...\n");
    getchar(); // ユーザー入力待ち
}

// メイン関数（テスト用）
// int main() {
//     // グローバル変数に初期データをセット
//     g_product_count = 3;
//     g_products[0].product_id = 1;
//     strcpy(g_products[0].product_name, "コーラ");
//     g_products[0].price = 120;
//     g_products[0].stock = 10;
//     g_products[0].is_active = 1;

//     g_products[1].product_id = 2;
//     strcpy(g_products[1].product_name, "お茶");
//     g_products[1].price = 100;
//     g_products[1].stock = 20;
//     g_products[1].is_active = 1;

//     g_products[2].product_id = 3;
//     strcpy(g_products[2].product_name, "水");
//     g_products[2].price = 80;
//     g_products[2].stock = 15;
//     g_products[2].is_active = 1;

//     // 商品一覧表示
//     displayProductList();

//     return 0;
// }
