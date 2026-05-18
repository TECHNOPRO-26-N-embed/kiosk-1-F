#include <stdio.h>
#include <string.h>
#include <time.h>

// 商品構造体
typedef struct {
    char id[10];      // 商品ID (例: D01)
    char name[50];    // 商品名
    int price;        // 料金
    int stock;        // 在庫数
} Product;

// ログ構造体
typedef struct {
    char timestamp[20];  // 日時
    char product_id[10]; // 商品ID
    char product_name[50]; // 商品名
    int price;          // 購入金額
} PurchaseLog;

/**
 * 機能4: 商品購入処理
 * 
 * @param product: 選択された商品
 * @param paid_money: 投入金額
 * @return 成功時は1, 失敗時は0
 */
int purchase_product(Product *product, int paid_money) {
    
    // ステップ1: 商品が売り切れでないか確認
    if (product->stock <= 0) {
        printf("申し訳ございません。この商品は売り切れです。\n");
        return 0;  // 失敗
    }
    
    // ステップ2: 入金額が足りているか確認
    if (paid_money < product->price) {
        printf("金額が不足しています。\n");
        printf("商品価格: %d円\n", product->price);
        printf("投入金額: %d円\n", paid_money);
        printf("不足額: %d円\n", product->price - paid_money);
        return 0;  // 失敗
    }
    
    // ステップ3: 購入処理 - 在庫を減らす
    product->stock--;
    printf("購入完了しました！\n");
    printf("商品: %s\n", product->name);
    printf("価格: %d円\n", product->price);
    printf("残在庫: %d\n", product->stock);
    
    // ステップ4: ログに記録（CSV形式）
    if (save_purchase_log(product, paid_money)) {
        printf("ログを保存しました。\n");
    } else {
        printf("警告: ログの保存に失敗しました。\n");
    }
    
    // ステップ5: おつり計算
    int change = paid_money - product->price;
    if (change > 0) {
        printf("\nおつり: %d円\n", change);
        printf("おつり処理へ移行します...\n");
        // ここでおつり処理（F06）を呼び出す
        // process_refund(change);
    }
    
    return 1;  // 成功
}

/**
 * 購入履歴をCSVログに保存
 * 
 * @param product: 購入された商品
 * @param paid_money: 支払金額
 * @return 成功時は1, 失敗時は0
 */
int save_purchase_log(Product *product, int paid_money) {
    FILE *fp;
    time_t now;
    struct tm *timeinfo;
    char timestamp[20];
    
    // 現在時刻を取得
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // ログファイルを追記モードで開く
    fp = fopen("purchase_log.csv", "a");
    if (fp == NULL) {
        printf("ログファイルを開けません。\n");
        return 0;
    }
    
    // ファイルが空の場合、ヘッダーを書き込む
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "購入日時,商品ID,商品名,購入価格\n");
    }
    
    // ログデータを書き込む
    fprintf(fp, "%s,%s,%s,%d\n", timestamp, product->id, product->name, product->price);
    
    fclose(fp);
    return 1;
}

// テスト用の例
/*
int main(void) {
    Product sample = {"D01", "コカ・コーラ", 120, 50};
    
    printf("=== 商品購入処理テスト ===\n\n");
    
    // テスト1: 正常な購入
    printf("テスト1: 入金額 >= 商品価格\n");
    purchase_product(&sample, 500);  // 500円投入
    
    printf("\n---\n\n");
    
    // テスト2: 金額不足
    printf("テスト2: 入金額 < 商品価格\n");
    purchase_product(&sample, 100);  // 100円投入（不足）
    
    printf("\n---\n\n");
    
    // テスト3: 売り切れ
    printf("テスト3: 売り切れの商品\n");
    sample.stock = 0;
    purchase_product(&sample, 500);
    
    return 0;
}
*/
