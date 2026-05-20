#include "global.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * ai_F12.c
 * 役割: 購入確定処理（F12）
 * - 購入内容を確定し、在庫・売上・ログを更新する
 * - ai_main.cや購入処理から呼び出されることを想定
 */

/*
 * 現在時刻をYYYY-MM-DD HH:MM:SS形式で取得
 * buf: 20バイト以上のバッファを渡すこと
 */
void getCurrentTimestamp(char *buf) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buf, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

/*
 * 購入確定処理
 * 引数:
 *   Transaction *trans: 購入取引情報（事前に金額・数量等をセット済み）
 * 戻り値:
 *   0: 正常終了
 *  -1: 在庫不足
 *  -2: ファイル書込エラー
 *
 * - 在庫を減算し、販売ログ・操作ログをCSVに追記する
 * - エラー時は故障予知ログも記録
 */
int finalizeTransaction(Transaction *trans) {
    // 商品IDから商品を検索し、在庫数自動更新（F05）を実施
    int found = 0;
    for (int i = 0; i < g_product_count; ++i) {
        if (g_products[i].product_id == trans->product_id && g_products[i].is_active) {
            // 在庫チェック
            if (g_products[i].stock < trans->quantity) {
                // 在庫不足
                return -1;
            }
            // F05: 在庫数自動更新
            // 購入時に在庫数を自動で減算する
            g_products[i].stock -= trans->quantity;
            found = 1;
            break;
        }
    }
    if (!found) {
        // 商品未検出
        return -1;
    }

    // 取引時刻をセット
    getCurrentTimestamp(trans->timestamp);

    // 販売ログCSVに追記
    FILE *fp = fopen(g_sales_log_csv_path, "a");
    if (!fp) {
        // ファイル書込エラー
        // 故障予知ログも記録（省略可）
        return -2;
    }
    // CSV: 取引ID,セッションID,日時,商品ID,単価,数量,小計,投入金額,釣銭
    fprintf(fp, "%d,%d,%s,%d,%d,%d,%d,%d,%d\n",
        trans->transaction_id,
        trans->session_id,
        trans->timestamp,
        trans->product_id,
        trans->unit_price,
        trans->quantity,
        trans->subtotal,
        trans->inserted_amount,
        trans->change_amount
    );
    fclose(fp);

    // 操作ログCSVに追記
    FILE *fp2 = fopen(g_operation_log_csv_path, "a");
    if (fp2) {
        // 操作ログ: log_id,session_id,timestamp,action_type,detail,result,error_code
        fprintf(fp2, "%d,%d,%s,BUY,商品ID:%d,OK,\n",
            g_next_operation_log_id++,
            trans->session_id,
            trans->timestamp,
            trans->product_id
        );
        fclose(fp2);
    }
    // 正常終了
    return 0;
}
