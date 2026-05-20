#ifndef AI_F14_H
#define AI_F14_H

#include "global.h"

/* F14: 操作ログCSVへ1件追記する */
int logOperation(const char *actionType, const char *detail, const char *result, const char *errorCode);

/* F14: 販売ログCSVへ1件追記する */
int logSale(const Transaction *tx, const char *result);

/* F14: 故障予知(エラー)CSVへ1件追記する */
int logErrorPrediction(const char *errorType,
                       const char *location,
                       const char *inputValue,
                       const char *message,
                       const char *severity);

/* 共通エラーハンドリング: 標準エラー出力 + 操作ログ + 故障予知ログに記録 */
int handleError(const char *errorType,
                const char *location,
                const char *inputValue,
                const char *message,
                const char *severity);

/* 起動時用: CSVファイル初期化のみ（画面表示なし） */
int initializeLogFiles(void);

/* メニューから呼び出すF14入口: ログファイル生成/確認と保存先案内 */
int showAndSaveLogs(void);

#endif