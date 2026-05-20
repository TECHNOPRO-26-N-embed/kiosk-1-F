#ifndef KIOSK_GLOBAL_H
#define KIOSK_GLOBAL_H

/* 要件の制約値（商品最大50件、在庫最大50） */
#define MAX_PRODUCTS 50
#define MAX_STOCK_PER_PRODUCT 50
#define MAX_NAME_LEN 64
#define MAX_DETAIL_LEN 128
#define MAX_ERROR_CODE_LEN 32
#define MAX_SEVERITY_LEN 16
#define MAX_PATH_LEN 260

/* 商品マスタ（F01-F06） */
typedef struct {
    /* 商品ID（1以上の連番を想定） */
    int product_id;
    /* 商品名 */
    char product_name[MAX_NAME_LEN];
    /* 単価 */
    int price;
    /* 在庫数（0〜MAX_STOCK_PER_PRODUCT） */
    int stock;
    /* 1: 有効商品 / 0: 無効商品（削除・入替済み） */
    int is_active;
} Product;

/* 1取引の情報（F07-F12） */
typedef struct {
    /* 取引ID */
    int transaction_id;
    /* セッションID（利用者識別） */
    int session_id;
    /* 購入対象の商品ID */
    int product_id;
    /* 購入数量 */
    int quantity;
    /* 購入時点の単価 */
    int unit_price;
    /* 小計（unit_price * quantity） */
    int subtotal;
    /* 投入金額 */
    int inserted_amount;
    /* 釣銭 */
    int change_amount;
    /* 取引時刻（YYYY-MM-DD HH:MM:SS を想定） */
    char timestamp[20];
} Transaction;

/* 操作ログ1件分（F14） */
typedef struct {
    /* 操作ログID */
    int log_id;
    /* セッションID */
    int session_id;
    /* 操作時刻 */
    char timestamp[20];
    /* 操作種別（例: REGISTER, BUY, DELETE） */
    char action_type[32];
    /* 操作詳細 */
    char detail[MAX_DETAIL_LEN];
    /* 結果（OK/NG） */
    char result[8];
    /* エラーコード（正常時は空文字可） */
    char error_code[MAX_ERROR_CODE_LEN];
} OperationLog;

/* 故障予知ログ1件分（F14） */
typedef struct {
    /* 記録ID */
    int record_id;
    /* 発生時刻 */
    char timestamp[20];
    /* エラー種別 */
    char error_type[32];
    /* 発生箇所 */
    char location[32];
    /* 問題入力値 */
    char input_value[32];
    /* メッセージ本文 */
    char message[MAX_DETAIL_LEN];
    /* 重大度（LOW/MEDIUM/HIGH など） */
    char severity[MAX_SEVERITY_LEN];
} ErrorPredictionLog;

/* F01-F06: product master and capacity management */
/* 商品マスタ本体。配列全体で最大MAX_PRODUCTS件を管理する。 */
extern Product g_products[MAX_PRODUCTS];
/* 現在登録されている有効商品数。 */
extern int g_product_count;

/* F07-F12: active purchase session data */
/* 購入処理中の一時取引情報（選択商品・数量・金額）。 */
extern Transaction g_current_transaction;

/* F13: sequential multi-user handling by session id */
/* 現在処理中の利用者セッションID。 */
extern int g_current_session_id;

/* IDs shared by sales, operation, and prediction logs */
/* 次に採番する取引ID。 */
extern int g_next_transaction_id;
/* 次に採番する操作ログID。 */
extern int g_next_operation_log_id;
/* 次に採番する故障予知記録ID。 */
extern int g_next_error_record_id;

/* F14: csv output destinations */
/* 販売ログCSVの出力先パス。 */
extern char g_sales_log_csv_path[MAX_PATH_LEN];
/* 操作ログCSVの出力先パス。 */
extern char g_operation_log_csv_path[MAX_PATH_LEN];
/* 故障予知CSVの出力先パス。 */
extern char g_error_prediction_csv_path[MAX_PATH_LEN];
/* エラーログCSVの出力先パス。 */
extern char g_error_log_csv_path[MAX_PATH_LEN];

#endif