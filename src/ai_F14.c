#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ai_F14.h"

/*
 * ai_F14.c
 * 役割:
 * - F14のログ管理を担当する
 * - 販売ログ/操作ログ/故障予知ログ/エラーログをCSVへ追記する
 * - エラー発生時に共通処理(handleError)で複数ログへ同時記録する
 */

#define TIMESTAMP_BUF_LEN 20

/* NULL安全に文字列をコピーする補助関数 */
static void copySafe(char *dst, size_t dstSize, const char *src)
{
	if (dstSize == 0) {
		return;
	}
	if (src == NULL) {
		dst[0] = '\0';
		return;
	}
	strncpy(dst, src, dstSize - 1);
	dst[dstSize - 1] = '\0';
}

/* 現在時刻を "YYYY-MM-DD HH:MM:SS" 形式で作る */
static void makeTimestamp(char *out, size_t outSize)
{
	time_t now;
	struct tm localTm;

	if (outSize == 0) {
		return;
	}

	now = time(NULL);
#if defined(_WIN32)
	localtime_s(&localTm, &now);
#else
	localTm = *localtime(&now);
#endif
	strftime(out, outSize, "%Y-%m-%d %H:%M:%S", &localTm);
}

/* CSV向けに文字列をクォート付きで出力（"は二重化） */
static void writeCsvEscaped(FILE *fp, const char *text)
{
	const char *p;

	if (text == NULL) {
		fputs("\"\"", fp);
		return;
	}

	fputc('"', fp);
	for (p = text; *p != '\0'; ++p) {
		if (*p == '"') {
			fputc('"', fp);
		}
		fputc(*p, fp);
	}
	fputc('"', fp);
}

/* 対象CSVが未作成ならヘッダ行付きで新規作成する */
static int ensureCsvHeader(const char *path, const char *header)
{
	FILE *fp;

	fp = fopen(path, "r");
	if (fp != NULL) {
		fclose(fp);
		return 0;
	}

	fp = fopen(path, "w");
	if (fp == NULL) {
		return -1;
	}
	fprintf(fp, "%s\n", header);
	fclose(fp);
	return 0;
}

/* 商品IDから商品名を検索する。見つからない場合はUNKNOWNを返す */
static const char *findProductNameById(int productId)
{
	int i;
	for (i = 0; i < g_product_count; i++) {
		if (g_products[i].is_active && g_products[i].product_id == productId) {
			return g_products[i].product_name;
		}
	}
	return "UNKNOWN";
}

/* 本機能で使う全CSVファイルを初期化（存在確認＋ヘッダ保証） */
static int ensureAllLogFiles(void)
{
	if (ensureCsvHeader(g_sales_log_csv_path,
						"transaction_id,session_id,timestamp,product_id,product_name,unit_price,quantity,subtotal,inserted_amount,change_amount,result") != 0) {
		return -1;
	}

	if (ensureCsvHeader(g_operation_log_csv_path,
						"log_id,session_id,timestamp,action_type,detail,result,error_code") != 0) {
		return -1;
	}

	if (ensureCsvHeader(g_error_prediction_csv_path,
						"record_id,timestamp,error_type,location,input_value,message,severity") != 0) {
		return -1;
	}

	if (ensureCsvHeader(g_error_log_csv_path,
						"error_id,session_id,timestamp,error_type,location,input_value,message,severity") != 0) {
		return -1;
	}

	return 0;
}

/* 操作ログを1件追記する（操作種別、詳細、結果、エラーコード） */
int logOperation(const char *actionType, const char *detail, const char *result, const char *errorCode)
{
	FILE *fp;
	OperationLog logEntry;

	if (ensureAllLogFiles() != 0) {
		return -1;
	}

	logEntry.log_id = g_next_operation_log_id++;
	logEntry.session_id = g_current_session_id;
	makeTimestamp(logEntry.timestamp, sizeof(logEntry.timestamp));
	copySafe(logEntry.action_type, sizeof(logEntry.action_type), actionType);
	copySafe(logEntry.detail, sizeof(logEntry.detail), detail);
	copySafe(logEntry.result, sizeof(logEntry.result), result);
	copySafe(logEntry.error_code, sizeof(logEntry.error_code), errorCode);

	fp = fopen(g_operation_log_csv_path, "a");
	if (fp == NULL) {
		return -1;
	}

	fprintf(fp, "%d,%d,", logEntry.log_id, logEntry.session_id);
	writeCsvEscaped(fp, logEntry.timestamp);
	fputc(',', fp);
	writeCsvEscaped(fp, logEntry.action_type);
	fputc(',', fp);
	writeCsvEscaped(fp, logEntry.detail);
	fputc(',', fp);
	writeCsvEscaped(fp, logEntry.result);
	fputc(',', fp);
	writeCsvEscaped(fp, logEntry.error_code);
	fputc('\n', fp);
	fclose(fp);

	return 0;
}

/* 販売ログを1件追記する（取引情報をCSV形式で保存） */
int logSale(const Transaction *tx, const char *result)
{
	FILE *fp;
	const char *productName;
	char timestamp[TIMESTAMP_BUF_LEN];

	if (tx == NULL) {
		return -1;
	}
	if (ensureAllLogFiles() != 0) {
		return -1;
	}

	if (tx->timestamp[0] == '\0') {
		makeTimestamp(timestamp, sizeof(timestamp));
	} else {
		copySafe(timestamp, sizeof(timestamp), tx->timestamp);
	}

	productName = findProductNameById(tx->product_id);

	fp = fopen(g_sales_log_csv_path, "a");
	if (fp == NULL) {
		return -1;
	}

	fprintf(fp,
			"%d,%d,",
			tx->transaction_id,
			tx->session_id);
	writeCsvEscaped(fp, timestamp);
	fprintf(fp, ",%d,", tx->product_id);
	writeCsvEscaped(fp, productName);
	fprintf(fp,
			",%d,%d,%d,%d,%d,",
			tx->unit_price,
			tx->quantity,
			tx->subtotal,
			tx->inserted_amount,
			tx->change_amount);
	writeCsvEscaped(fp, result != NULL ? result : "OK");
	fputc('\n', fp);
	fclose(fp);

	return 0;
}

/* 故障予知ログを1件追記する（将来分析向けのエラー情報） */
int logErrorPrediction(const char *errorType,
					   const char *location,
					   const char *inputValue,
					   const char *message,
					   const char *severity)
{
	FILE *fp;
	ErrorPredictionLog record;

	if (ensureAllLogFiles() != 0) {
		return -1;
	}

	record.record_id = g_next_error_record_id++;
	makeTimestamp(record.timestamp, sizeof(record.timestamp));
	copySafe(record.error_type, sizeof(record.error_type), errorType);
	copySafe(record.location, sizeof(record.location), location);
	copySafe(record.input_value, sizeof(record.input_value), inputValue);
	copySafe(record.message, sizeof(record.message), message);
	copySafe(record.severity, sizeof(record.severity), severity);

	fp = fopen(g_error_prediction_csv_path, "a");
	if (fp == NULL) {
		return -1;
	}

	fprintf(fp, "%d,", record.record_id);
	writeCsvEscaped(fp, record.timestamp);
	fputc(',', fp);
	writeCsvEscaped(fp, record.error_type);
	fputc(',', fp);
	writeCsvEscaped(fp, record.location);
	fputc(',', fp);
	writeCsvEscaped(fp, record.input_value);
	fputc(',', fp);
	writeCsvEscaped(fp, record.message);
	fputc(',', fp);
	writeCsvEscaped(fp, record.severity);
	fputc('\n', fp);
	fclose(fp);

	return 0;
}

/* エラーログCSVに1件追記する（障害追跡用） */
static int logError(const char *errorType,
					const char *location,
					const char *inputValue,
					const char *message,
					const char *severity)
{
	FILE *fp;
	int errorId;
	char timestamp[TIMESTAMP_BUF_LEN];

	if (ensureAllLogFiles() != 0) {
		return -1;
	}

	errorId = g_next_error_record_id;
	makeTimestamp(timestamp, sizeof(timestamp));

	fp = fopen(g_error_log_csv_path, "a");
	if (fp == NULL) {
		return -1;
	}

	fprintf(fp, "%d,%d,", errorId, g_current_session_id);
	writeCsvEscaped(fp, timestamp);
	fputc(',', fp);
	writeCsvEscaped(fp, errorType);
	fputc(',', fp);
	writeCsvEscaped(fp, location);
	fputc(',', fp);
	writeCsvEscaped(fp, inputValue);
	fputc(',', fp);
	writeCsvEscaped(fp, message);
	fputc(',', fp);
	writeCsvEscaped(fp, severity);
	fputc('\n', fp);
	fclose(fp);

	return 0;
}

/*
 * 共通エラーハンドリング:
 * 1) 標準エラー出力へ表示
 * 2) 操作ログへERRORとして記録
 * 3) 故障予知ログへ記録
 * 4) エラーログへ記録
 */
int handleError(const char *errorType,
				const char *location,
				const char *inputValue,
				const char *message,
				const char *severity)
{
	fprintf(stderr,
			"[ERROR] type=%s location=%s input=%s message=%s severity=%s\n",
			errorType != NULL ? errorType : "",
			location != NULL ? location : "",
			inputValue != NULL ? inputValue : "",
			message != NULL ? message : "",
			severity != NULL ? severity : "");

	(void)logOperation("ERROR",
					   message != NULL ? message : "",
					   "NG",
					   errorType != NULL ? errorType : "UNKNOWN");
	(void)logErrorPrediction(errorType, location, inputValue, message, severity);
	(void)logError(errorType, location, inputValue, message, severity);

	return -1;
}

/* F14メニュー用の入口。ログファイル初期化と保存先表示を行う */
int showAndSaveLogs(void)
{
	if (ensureAllLogFiles() != 0) {
		return handleError("LOG_IO_ERROR",
						   "showAndSaveLogs",
						   "",
						   "failed to initialize log csv files",
						   "HIGH");
	}

	printf("\n=== CSV保存先 ===\n");
	printf("販売ログ: %s\n", g_sales_log_csv_path);
	printf("操作ログ: %s\n", g_operation_log_csv_path);
	printf("故障予知ログ: %s\n", g_error_prediction_csv_path);
	printf("エラーログ: %s\n", g_error_log_csv_path);

	(void)logOperation("SHOW_LOG_PATHS",
					   "displayed csv output destinations",
					   "OK",
					   "");
	return 0;
}
