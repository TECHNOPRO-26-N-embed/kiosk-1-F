#include "global.h"
#include <stdio.h>

/*
 * ai_F11.c
 * 役割: 釣銭計算・返却（F11）
 * - 購入処理の一部として、投入金額と小計から釣銭を計算し返却金額を返す
 * - ai_main.cや購入処理（F05-F12）から呼び出されることを想定
 */

/*
 * 釣銭計算関数
 * 引数:
 *   inserted_amount: ユーザーが投入した金額（円）
 *   subtotal: 購入金額の合計（円）
 * 戻り値:
 *   釣銭金額（円）。投入金額が不足している場合は-1を返す。
 *
 * 例: inserted_amount=1000, subtotal=750 → 戻り値: 250
 */
int calculateChange(int inserted_amount, int subtotal) {
    // 入力値の妥当性チェック
    if (inserted_amount < 0 || subtotal < 0) {
        // 異常値（負の金額）はエラー扱い
        return -1;
    }
    if (inserted_amount < subtotal) {
        // 投入金額が不足している場合
        return -1;
    }
    // 釣銭を計算
    int change = inserted_amount - subtotal;
    // 釣銭金額を返却
    return change;
}

/*
 * 釣銭返却処理（オプション: 釣銭の内訳表示などを追加する場合はこちら）
 * 必要に応じて拡張可能
 */
void printChange(int change) {
    if (change > 0) {
        printf("\n[案内] 釣銭: %d 円を返却します。\n", change);
    } else if (change == 0) {
        printf("\n[案内] 釣銭はありません。\n");
    } else {
        printf("\n[エラー] 釣銭計算に失敗しました。\n");
    }
}
