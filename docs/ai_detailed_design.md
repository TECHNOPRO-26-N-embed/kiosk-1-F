# 🛠️ 設計ワークシート

> **使い方**: このテンプレートをコピーして、チームの設計内容を記入してください。
> Day 1〜2（手作業フェーズ）で手書きした内容を、最終的にこのMarkdownに清書してGit提出します。

---

## 1. プロジェクト情報

| 項目 | 記入欄 |
| :--- | :--- |
| **チーム名** | |
| **選択テーマ（A〜C）** | |
| **アプリ名** | 自動販売機システム |

---

## 2. 要求仕様書（顧客ヒアリング結果）

### 2.1 顧客の要望（ヒアリングメモ）

> ここに、顧客（講師）へのヒアリングで得た要望を箇条書きで記録してください。

* 商品一覧を確認できること
* 商品の登録・編集・削除ができること
* 購入時に在庫が自動で減算されること
* 金額投入と釣銭計算ができること
* 失敗入力でもプログラムが停止せず再入力を促すこと
* 取引や操作のログをCSV形式で記録すること

### 2.2 機能要件

| 機能ID | 機能名             | 概要                                                    |
|:------:|:------------------|:--------------------------------------------------------|
| F01    | 商品一覧表示         | 登録されている商品の一覧（商品名・価格・在庫数）を表示する |
| F02    | 商品登録             | 新しい商品を登録する                                   |
| F03    | 商品情報編集         | 既存商品の情報（商品名・価格・在庫数）を編集する          |
| F04    | 商品削除             | 商品を削除する                                         |
| F05    | 在庫数自動更新       | 購入時に在庫数を自動で減算する                           |
| F06    | 商品入れ替え         | 商品の入れ替え（新商品への差し替え）に対応する            |
| F07    | 商品選択             | ユーザーが購入する商品を選択する                         |
| F08    | 数量入力             | ユーザーが購入する数量を入力する                         |
| F09    | 合計金額表示         | 選択商品・数量に応じた合計金額を表示する                  |
| F10    | 現金投入受付         | ユーザーから現金を受け付ける                             |
| F11    | 釣銭計算・返却       | 必要に応じて釣銭を計算し返却する                          |
| F12    | 購入確定             | 購入内容を確定し、在庫・売上・ログを更新する              |
| F13    | 複数客対応           | 複数のユーザーが同時に利用できるようにする                |
| F14    | ログ保存             | 販売履歴や操作ログ、故障予知用データをCSV形式で保存        |

---

## 3. 基本設計



### 3.1 画面遷移図

> テキスト形式で画面の状態遷移を記述してください。
> 例: `メインメニュー → [1] → 購入画面 → [確定] → 完了画面 → メインメニュー`

```
メインメニュー → [1] → 商品一覧表示 → 戻る → メインメニュー
メインメニュー → [2] → 商品登録 → 確定/キャンセル → メインメニュー
メインメニュー → [3] → 商品編集 → 確定/キャンセル → メインメニュー
メインメニュー → [4] → 商品削除 → 確定/キャンセル → メインメニュー
メインメニュー → [5] → 購入処理 → 確定/キャンセル → メインメニュー
メインメニュー → [6] → ログ表示/CSV保存 → 戻る → メインメニュー
メインメニュー → [9] → 終了
```

## 3. 非機能要件

| 区分 | 要件 |
| :--- | :--- |
| 可用性 | 入力異常やログ書き込み失敗が発生しても、プログラム全体は終了せずメニューに復帰できること。 |
| 性能 | 通常操作（メニュー表示、商品選択、購入確定）の応答は1秒以内を目標とする。 |
| 保守性 | 商品数上限（50件）や税計算方式などの定数を、1か所の定義変更で反映できる構造にする。 |
| ログ品質 | 全ログに日時、操作種別、結果（OK/NG）、セッションIDを含めること。 |
| 操作性 | 初見利用者でも、3ステップ以内（選択→数量→投入）で購入完了できること。 |

## 4. 制約条件・前提

| 項目 | 内容 |
| :--- | :--- |
| 商品種類数 | 同時に管理できる商品は最大50種類。 |
| 在庫数 | 各商品の在庫は0以上50以下。 |
| 入力方式 | キーボードによる数値入力を基本とし、無効入力時は再入力させる。 |
| データ保存 | 永続化はCSVファイルで行い、文字コードはUTF-8を使用する。 |
| 同時利用 | 複数利用者はセッション単位で順次処理し、同一在庫への更新競合を防ぐ。 |

## 5. 受け入れ基準

| ID | 受け入れ基準 |
| :--: | :--- |
| A01 | 商品登録時、51件目の追加は拒否され、エラーメッセージが表示される。 |
| A02 | 在庫を超える数量入力は確定されず、再入力を求める。 |
| A03 | 購入確定時に合計金額と釣銭が正しく表示され、在庫が減算される。 |
| A04 | 1取引ごとに販売ログCSVと操作ログCSVへ1行以上追記される。 |
| A05 | 数字以外入力や不正商品ID入力でもプログラムが停止せず、メニューに戻れる。 |
| A06 | エラーログには故障予知用としてエラー種別と発生時刻が記録される。 |

## 6. データ設計

| データ名 | 機能要件対応 | 内容・説明 |
| :--- | :--- | :--- |
| 商品マスタ | F01,F02,F03,F04,F06 | 商品ID、商品名、価格、在庫数、有効/無効（最大50件） |
| 取引情報 | F05,F07,F08,F09,F10,F11,F12 | 取引ID、セッションID、商品ID、数量、合計金額、投入金額、釣銭、日時 |
| 操作ログ | F14 | ログID、セッションID、日時、操作種別、詳細、結果、エラーコード |
| 故障予知データ | F14 | 記録ID、日時、エラー種別、発生箇所、入力値、メッセージ、重大度 |
| セッション情報 | F13 | 複数利用者を区別するための利用単位情報 |

## 7. CSV保存形式

- 販売ログCSV: 取引ID,セッションID,日時,商品ID,商品名,単価,数量,小計,投入金額,釣銭,結果
- 操作ログCSV: ログID,セッションID,日時,操作種別,詳細,結果,エラーコード
- 故障予知CSV: 記録ID,日時,エラー種別,発生箇所,入力値,メッセージ,重大度

## 8. 画面遷移図

完全遷移: メインメニューを起点に、購入フローと管理フローを選択できる。

<pre class="mermaid">
flowchart TD
	M[メインメニュー]

	M -->|[1] 購入| P1[購入画面: 商品一覧]
	P1 -->|商品選択| P2[数量入力]
	P2 -->|決定| P3[合計金額表示]
	P3 -->|現金投入| P4[投入金額確認]
	P4 -->|不足| P3
	P4 -->|充足| P5[釣銭計算]
	P5 -->|[確定]| P6[完了画面]
	P6 -->|[メニューへ戻る]| M

	M -->|[2] 管理| A0[管理メニュー]
	A0 -->|[1] 商品登録| A1[商品登録画面]
	A0 -->|[2] 商品編集| A2[商品編集画面]
	A0 -->|[3] 商品削除| A3[商品削除画面]
	A0 -->|[4] 商品入れ替え| A4[商品入れ替え画面]
	A1 -->|保存| A5[完了メッセージ]
	A2 -->|保存| A5
	A3 -->|実行| A5
	A4 -->|実行| A5
	A5 -->|[管理メニューへ戻る]| A0
	A0 -->|[0] メインへ戻る| M

	M -->|[0] 終了| E[終了]
</pre>



```

---

## 4. 詳細設計（アルゴリズム）

### 4.1 グローバル変数定義（ファイル間で共有）

```c
// ===== 定数定義 =====
#define MAX_PRODUCTS      50        // 商品マスタの最大登録数
#define MAX_NAME_LEN      100       // 商品名の最大文字数
#define MAX_SESSION_LEN   20        // セッションIDの最大文字数
#define MAX_LOG_BUFFER    1000      // ログ出力バッファのサイズ
#define MAX_CSV_LINE      500       // CSV行の最大文字数

// ===== 商品マスタ構造体 =====
typedef struct {
    int productId;              // 商品ID
    char productName[MAX_NAME_LEN];  // 商品名
    int price;                  // 単価（円）
    int stock;                  // 在庫数
    int isValid;                // 有効フラグ（1:有効, 0:無効）
} Product;

// ===== 取引情報構造体 =====
typedef struct {
    int transactionId;          // 取引ID
    char sessionId[MAX_SESSION_LEN];  // セッションID
    int productId;              // 商品ID
    char productName[MAX_NAME_LEN];   // 商品名
    int price;                  // 単価
    int quantity;               // 購入数量
    int subtotal;               // 小計金額
    int amountReceived;         // 投入金額
    int change;                 // 釣銭
    char timestamp[20];         // タイムスタンプ（YYYY-MM-DD HH:MM:SS）
    char result[10];            // 処理結果（OK/NG）
} Transaction;

// ===== 操作ログ構造体 =====
typedef struct {
    int logId;                  // ログID
    char sessionId[MAX_SESSION_LEN];  // セッションID
    char timestamp[20];         // タイムスタンプ
    char operationType[30];     // 操作種別（"商品表示", "登録", "購入"等）
    char details[200];          // 詳細情報
    char result[10];            // 結果（OK/NG）
    int errorCode;              // エラーコード
} OperationLog;

// ===== グローバル変数 =====
Product g_productMaster[MAX_PRODUCTS];  // 商品マスタ配列
int g_productCount = 0;                 // 登録済み商品数
int g_nextProductId = 1;                // 次の商品IDカウンター
int g_nextTransactionId = 1;            // 次の取引IDカウンター
int g_nextLogId = 1;                    // 次のログIDカウンター
char g_sessionId[MAX_SESSION_LEN];      // 現在のセッションID
char g_timestamp[20];                   // 現在のタイムスタンプ

// ===== ファイルパス定数 =====
#define PRODUCT_CSV         "data/products.csv"
#define TRANSACTION_LOG     "data/transaction_log.csv"
#define OPERATION_LOG       "data/operation_log.csv"
#define FAILURE_PREDICT_LOG "data/failure_predict.csv"
```

### 4.2 メインループのアルゴリズムと関数定義

#### 4.2.1 メインループの流れ

```
1. 起動時に初期化処理を実行（商品マスタ読込、セッションID生成）
2. メインメニューを表示する
3. ユーザーの入力を受け取る
4. 入力に応じて処理を分岐する
   - 1: 商品一覧表示
   - 2: 商品登録
   - 3: 商品情報編集
   - 4: 商品削除
   - 5: 購入処理
   - 6: ログ表示/CSV保存
   - 9: 終了
5. 各処理終了後、メインメニューに戻る
6. 9が入力されたら保存処理を行いプログラムを終了する
```

#### 4.2.2 主要関数定義

| 関数名 | 引数 | 戻り値 | 説明 |
| :--- | :--- | :--- | :--- |
| `main()` | なし | `int` | プログラムのエントリポイント。メインループを制御する |
| `initializeSystem()` | なし | `void` | システム初期化（商品CSVロード、セッションID生成） |
| `displayMainMenu()` | なし | `int` | メインメニュー表示。ユーザー選択を返す |
| `getValidInput(min, max)` | `int min, int max` | `int` | 指定範囲の有効な整数入力を取得 |
| `getInputString(maxLen)` | `int maxLen` | `char*` | 入力文字列を動的に取得 |
| `getCurrentTimestamp(buf)` | `char* buf` | `void` | 現在の日時をYYYY-MM-DD HH:MM:SS形式で取得 |

### 4.3 個別機能のアルゴリズムと関数定義

#### 機能1: 商品一覧表示（F01）

**関数定義:**
```c
// 引数: なし
// 戻り値: int (0: 成功, -1: エラー)
// 説明: 登録されている商品をテーブル形式で表示する
int displayProductList(void);
```

**アルゴリズム:**
```
1. "商品マスタ一覧" というヘッダを表示
2. テーブルヘッダ（ID, 商品名, 価格, 在庫）を表示
3. g_productMaster[] を先頭から順に走査
   - isValid == 1 のみ表示
   - 1件ずつ見やすく表示
4. 一覧表示後、"任意キーでメニューに戻る" と促す
5. getchar() で入力待機
6. 操作ログに記録（OperationLog）
7. 成功時は 0、エラーは -1 を返す
```

#### 機能2: 商品登録（F02）

**関数定義:**
```c
// 引数: なし
// 戻り値: int (0: 成功, -1: 登録上限超過, -2: 入力エラー)
// 説明: 新規商品をマスタに登録する
int registerProduct(void);
```

**アルゴリズム:**
```
1. g_productCount >= MAX_PRODUCTS なら上限超過エラー（戻り値 -1）
2. "商品名を入力してください:" と表示
3. getInputString(MAX_NAME_LEN - 1) で商品名を入力
4. "単価を入力してください:" と表示
5. getValidInput(1, 1000000) で価格を入力
6. "在庫数を入力してください:" と表示
7. getValidInput(0, 50) で在庫数を入力
8. 入力値を検証
   - 商品名が空でないか確認
   - 価格 >= 1 か確認
   - 在庫数 >= 0 か確認
9. 検証OK なら新規Product構造体をg_productMaster[]に追加
   - productId = g_nextProductId++
   - isValid = 1
   - 他のフィールドに入力値を代入
   - g_productCount++
10. 操作ログに記録
11. "登録完了" メッセージ表示
12. 成功時は 0、エラーは -1 または -2 を返す
```

#### 機能3: 商品情報編集（F03）

**関数定義:**
```c
// 引数: なし
// 戻り値: int (0: 成功, -1: 商品未検出, -2: 入力エラー)
// 説明: 既存商品の情報（商品名・価格・在庫数）を編集する
int editProduct(void);
```

**アルゴリズム:**
```
1. "編集対象の商品IDを入力:" と表示
2. getValidInput(1, g_nextProductId - 1) で商品IDを入力
3. 商品IDでg_productMaster[]を検索
   - 見つからないか isValid == 0 なら未検出エラー（戻り値 -1）
4. 現在の商品情報を表示
5. "編集する項目を選択 [1]商品名 [2]価格 [3]在庫:" と表示
6. getValidInput(1, 3) で選択を入力
7. 選択に応じて新しい値を入力・検証
   - 商品名: getInputString()
   - 価格: getValidInput(1, 1000000)
   - 在庫: getValidInput(0, 50)
8. データ更新
9. 操作ログに記録
10. "編集完了" メッセージ表示
11. 成功時は 0、エラーは -1 または -2 を返す
```

#### 機能4: 商品削除（F04）

**関数定義:**
```c
// 引数: なし
// 戻り値: int (0: 成功, -1: 商品未検出, -2: キャンセル)
// 説明: 指定した商品をマスタから削除する（論理削除）
int deleteProduct(void);
```

**アルゴリズム:**
```
1. "削除対象の商品IDを入力:" と表示
2. getValidInput(1, g_nextProductId - 1) で商品IDを入力
3. 商品IDでg_productMaster[]を検索
   - 見つからないか isValid == 0 なら未検出エラー（戻り値 -1）
4. 削除対象の商品情報を表示
5. "本当に削除しますか？ [1]はい [2]いいえ:" と表示
6. getValidInput(1, 2) で確認入力
   - 2（いいえ）なら キャンセル（戻り値 -2）
7. 1（はい）なら該当商品の isValid = 0（論理削除）
8. g_productCount--
9. 操作ログに記録
10. "削除完了" メッセージ表示
11. 戻り値 0 を返す
```

#### 機能5: 購入処理（F05-F12）

**関数定義:**
```c
// 引数: なし
// 戻り値: int (0: 購入成功, -1: キャンセル, -2: エラー)
// 説明: 商品選択→数量入力→金額確認→現金投入→釣銭計算→ログ保存を統括
int executePurchase(void);
```

**サブ関数群:**
```c
// 戻り値: int (購入商品ID, または -1 でキャンセル)
// 説明: 商品一覧を表示し、ユーザーが選択した商品IDを返す
int selectProduct(void);

// 引数: int productId, int* quantityOut
// 戻り値: int (0: 成功, -1: キャンセル, -2: 在庫不足)
// 説明: 購入数量を入力・検証し、quantityOut に代入
int inputQuantity(int productId, int* quantityOut);

// 引数: int price, int quantity
// 戻り値: int (合計金額)
// 説明: 単価×数量 の合計金額を計算・表示
int calculateSubtotal(int price, int quantity);

// 引数: int subtotal, int* amountReceivedOut
// 戻り値: int (0: 充足, -1: 不足)
// 説明: 現金投入を受け付け、amountReceivedOut に代入
int receivePayment(int subtotal, int* amountReceivedOut);

// 引数: int amountReceived, int subtotal
// 戻り値: int (釣銭金額)
// 説明: 釣銭を計算
int calculateChange(int amountReceived, int subtotal);

// 引数: Transaction* trans
// 戻り値: int (0: 成功, -1: エラー)
// 説明: 在庫を減算し、取引ログを記録
int finalizeTransaction(Transaction* trans);
```

**購入処理のアルゴリズム:**
```
1. displayProductList() で商品一覧を表示
2. selectProduct() で商品を選択 → productId を取得
   - キャンセル（戻り値 -1）なら return -1
3. inputQuantity(productId, &quantity) で購入数量を入力
   - 在庫不足（戻り値 -2）なら "在庫不足" と表示して2.に戻る
4. subtotal = calculateSubtotal(price, quantity)
5. receivePayment(subtotal, &amountReceived)
   - 不足（戻り値 -1）なら 差額を表示して5.に戻る
6. change = calculateChange(amountReceived, subtotal)
7. 取引情報をTransaction構造体に格納
8. 在庫を減算：g_productMaster[].stock -= quantity
9. finalizeTransaction(&trans) でログ保存
10. "購入完了" メッセージと釣銭を表示
11. return 0
```

#### 機能6: ログ保存と異常ログ記録（F14）

**関数定義:**
```c
// 引数: OperationLog* log
// 戻り値: int (0: 成功, -1: ファイルオープンエラー)
// 説明: 操作ログをCSVファイルに追記
int appendOperationLog(OperationLog* log);

// 引数: Transaction* trans
// 戻り値: int (0: 成功, -1: ファイルオープンエラー)
// 説明: 取引ログをCSVファイルに追記
int appendTransactionLog(Transaction* trans);

// 引数: const char* errorType, const char* location, 
//       const char* inputValue, const char* message, int severity
// 戻り値: int (0: 成功, -1: ファイルオープンエラー)
// 説明: 故障予知用エラーログをCSVに記録
int appendFailurePredictLog(const char* errorType, const char* location,
                            const char* inputValue, const char* message, int severity);
```

**ログ保存のアルゴリズム:**
```
1. 各操作・取引の直後にcurrentTimestamp()で日時を取得
2. 操作ログ/取引ログ/エラーログの該当構造体を作成
3. appendOperationLog(), appendTransactionLog() でCSVに追記
4. 書き込み成功 → OperationLog.result = "OK"
5. 書き込み失敗 → OperationLog.result = "NG", errorCode = ファイルエラーコード
6. エラー時は appendFailurePredictLog() で故障予知ログに記録
7. ファイルI/O失敗でもプログラムは続行し、メニューに復帰
```

### 4.4 エラーハンドリング（想定される異常系）

| 異常系シナリオ | 対処方針 | 関連関数 |
| :--- | :--- | :--- |
| 数字以外の文字が入力された場合 | `getValidInput()` で再入力を促す | `getValidInput()` |
| 存在しない商品IDが入力された場合 | 商品検索に失敗、エラーメッセージ表示＆再入力 | `selectProduct()`, `editProduct()`, `deleteProduct()` |
| 在庫を超える数量が入力された場合 | `inputQuantity()` で検証、再入力を促す | `inputQuantity()` |
| 投入金額が合計より少ない場合 | `receivePayment()` で差額を表示＆再入力 | `receivePayment()` |
| CSVファイルが書き込み不可の場合 | `appendOperationLog()` 等がエラーコード-1を返す。エラーログに記録後、メニューに復帰 | `appendOperationLog()`, `appendTransactionLog()`, `appendFailurePredictLog()` |
| 商品数が50件に達した場合 | `registerProduct()` で -1（上限超過）を返す | `registerProduct()` |
| メモリ不足エラー | malloc失敗時は `getInputString()` で NULL を返す。呼び出し元で チェック＆復帰 | `getInputString()` |

---

## 5. 備考・メモ

> 設計中に気になった点や、明日の実装に向けた注意点があれば記録してください。

* 商品数上限は50件なので登録時に件数チェックを必ず入れる
* CSVはUTF-8で扱い、改行コードはOSに依存しない形式を意識する
* 入力検証は各画面で共通関数化し、エラー時には必ずメニューに戻れるようにする
* 操作ログと故障予知ログを分けて、通常の操作と異常系を区別する
* 画面表示は簡易テキストUIでも良いので、操作手順を3ステップ以内に収める
