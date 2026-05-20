#include "global.h"

/* 商品マスタ領域（起動時は空） */
Product g_products[MAX_PRODUCTS] = {0};
/* 初期状態では登録商品0件 */
int g_product_count = 0;

/* 購入中取引のワーク領域 */
Transaction g_current_transaction = {0};

/* セッションIDは処理開始時に採番して設定する */
int g_current_session_id = 0;

/* 各ログ/取引の採番は1から開始 */
int g_next_transaction_id = 1;
int g_next_operation_log_id = 1;
int g_next_error_record_id = 1;

/* CSV出力先のデフォルト設定 */
char g_sales_log_csv_path[MAX_PATH_LEN] = "data/sales_log_utf8.csv";
char g_operation_log_csv_path[MAX_PATH_LEN] = "data/operation_log.csv";
char g_error_prediction_csv_path[MAX_PATH_LEN] = "data/error_prediction.csv";
char g_error_log_csv_path[MAX_PATH_LEN] = "data/error_log.csv";