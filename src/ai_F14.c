#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>

#if defined(_WIN32)
#include <direct.h>
#endif

#include "ai_F14.h"

/*
 * ai_F14.c
 * 役割:
 * - F14のログ管理を担当する
 * - 販売ログ/操作ログ/故障予知ログ/エラーログをCSVへ追記する
 * - エラー発生時に共通処理(handleError)で複数ログへ同時記録する
 */

#define TIMESTAMP_BUF_LEN 20
#define LOG_PREVIEW_LINES 6
#define LOG_LINE_BUF_LEN 512

// 文字列が特定の接頭辞で始まるか確認する関数
// - 引数: text（対象文字列）, prefix（接頭辞）
// - 戻り値: 1（接頭辞で始まる場合）、0（それ以外）
static int startsWith(const char *text, const char *prefix)
{
	if (text == NULL || prefix == NULL) {
		return 0;
	}
	while (*prefix != '\0') {
		if (*text != *prefix) {
			return 0;
		}
		text++;
		prefix++;
	}
	return 1;
}

// ディレクトリが存在するか確認する関数
// - 引数: path（ディレクトリパス）
// - 戻り値: 1（存在する場合）、0（存在しない場合）
static int directoryExists(const char *path)
{
	struct stat st;
	if (path == NULL) {
		return 0;
	}
	if (stat(path, &st) != 0) {
		return 0;
	}
	return (st.st_mode & S_IFDIR) != 0;
}

// ディレクトリが存在しない場合、必要に応じて作成する関数
// - 引数: dir（ディレクトリパス）
// - 戻り値: 0（成功）、-1（失敗）
static int createDirectoryIfNeeded(const char *dir)
{
	if (dir == NULL || dir[0] == '\0') {
		return 0;
	}
	if (directoryExists(dir)) {
		return 0;
	}
#if defined(_WIN32)
	if (_mkdir(dir) == 0 || errno == EEXIST) {
		return 0;
	}
#else
	if (mkdir(dir, 0755) == 0 || errno == EEXIST) {
		return 0;
	}
#endif
	return -1;
}

// ファイルの親ディレクトリが存在しない場合、必要に応じて作成する関数
// - 引数: path（ファイルパス）
// - 戻り値: 0（成功）、-1（失敗）
static int ensureParentDirectory(const char *path)
{
	char dir[MAX_PATH_LEN];
	const char *slashPos;
	size_t len;

	if (path == NULL) {
		return -1;
	}

	slashPos = strrchr(path, '/');
	if (slashPos == NULL) {
		return 0;
	}

	len = (size_t)(slashPos - path);
	if (len == 0 || len >= sizeof(dir)) {
		return -1;
	}

	memcpy(dir, path, len);
	dir[len] = '\0';
	return createDirectoryIfNeeded(dir);
}

// データパスベースの調整
// - 引数: path（パス）、pathSize（パスの長さ）
// - 戻り値: 無
static void adjustDataPathBase(char *path, size_t pathSize)
{
	char resolved[MAX_PATH_LEN];

	if (path == NULL || pathSize == 0) {
		return;
	}

	if (startsWith(path, "data/") && !directoryExists("data") && directoryExists("../data")) {
		snprintf(resolved, sizeof(resolved), "../%s", path);
		strncpy(path, resolved, pathSize - 1);
		path[pathSize - 1] = '\0';
		return;
	}

	if (startsWith(path, "../data/") && !directoryExists("../data") && directoryExists("data")) {
		strncpy(path, path + 3, pathSize - 1);
		path[pathSize - 1] = '\0';
	}
}

// 全ログパスの調整
// - 引数: 無
// - 戻り値: 無
static void normalizeAllLogPaths(void)
{
	adjustDataPathBase(g_sales_log_csv_path, sizeof(g_sales_log_csv_path));
	adjustDataPathBase(g_operation_log_csv_path, sizeof(g_operation_log_csv_path));
	adjustDataPathBase(g_error_prediction_csv_path, sizeof(g_error_prediction_csv_path));
	adjustDataPathBase(g_error_log_csv_path, sizeof(g_error_log_csv_path));
}

// CSVプレビューの表示
// - 引数: title（タイトル）、path（ファイルパス）
// - 戻り値: 無
static void printCsvPreview(const char *title, const char *path)
{
	FILE *fp;
	char lines[LOG_PREVIEW_LINES][LOG_LINE_BUF_LEN];
	char line[LOG_LINE_BUF_LEN];
	int count = 0;
	int start;

	printf("\n[%s]\n", title);
	printf("path: %s\n", path);

	fp = fopen(path, "r");
	if (fp == NULL) {
		printf("(読み込み失敗)\n");
		return;
	}

	while (fgets(line, sizeof(line), fp) != NULL) {
		strncpy(lines[count % LOG_PREVIEW_LINES], line, LOG_LINE_BUF_LEN - 1);
		lines[count % LOG_PREVIEW_LINES][LOG_LINE_BUF_LEN - 1] = '\0';
		count++;
	}
	fclose(fp);

	if (count == 0) {
		printf("(空ファイル)\n");
		return;
	}

	if (count <= LOG_PREVIEW_LINES) {
		start = 0;
	} else {
		start = count - LOG_PREVIEW_LINES;
	}

	for (int i = start; i < count; i++) {
		printf("%s", lines[i % LOG_PREVIEW_LINES]);
	}
}

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

	if (ensureParentDirectory(path) != 0) {
		return -1;
	}

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
	normalizeAllLogPaths();

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
	_setmode(_fileno(fp), _O_U8TEXT); // Set UTF-8 mode

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
	_setmode(_fileno(fp), _O_U8TEXT); // Set UTF-8 mode

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
	_setmode(_fileno(fp), _O_U8TEXT); // Set UTF-8 mode

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
	_setmode(_fileno(fp), _O_U8TEXT); // Set UTF-8 mode

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

/* 起動時用: CSV生成/確認のみ実施（表示なし） */
int initializeLogFiles(void)
{
	if (ensureAllLogFiles() != 0) {
		return -1;
	}
	return 0;
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

	printf("\n=== CSV内容プレビュー（末尾%d行）===\n", LOG_PREVIEW_LINES);
	printCsvPreview("販売ログ", g_sales_log_csv_path);
	printCsvPreview("操作ログ", g_operation_log_csv_path);
	printCsvPreview("故障予知ログ", g_error_prediction_csv_path);
	printCsvPreview("エラーログ", g_error_log_csv_path);

	(void)logOperation("SHOW_LOG_PATHS",
					   "displayed csv output destinations",
					   "OK",
					   "");
	return 0;
}
