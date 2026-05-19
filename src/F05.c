// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_LINE 256   // 1行の最大文字数
// #define MAX_NAME 100   // 商品名の最大文字数

// int main() {

//     // ユーザーが入力する商品名
//     char input[MAX_NAME];
//     printf("Enter a name: ");
//     scanf("%s", input);

//     // 元ファイル（読み込み）と一時ファイル（書き込み）
//     FILE *fp = fopen("stock.txt", "r");
//     FILE *tmp = fopen("stock_tmp.txt", "w");

//     // ファイルが開けない場合のエラー処理
//     if (fp == NULL || tmp == NULL) {
//         printf("Error opening file.\n");
//         return 1;
//     }

//     char linebuf[MAX_LINE];  // ファイルから1行読み込むバッファ

//     // ファイルを1行ずつ読み込む
//     while (fgets(linebuf, sizeof(linebuf), fp)) {

//         char name[MAX_NAME]; // 商品名
//         int stock;           // 在庫数

//         // CSV形式（商品名,在庫数）からデータを取り出す
//         sscanf(linebuf, "%[^,],%d", name, &stock);

//         // 念のため改行を削除
//         name[strcspn(name, "\n")] = '\0';

//         // 入力された商品名と一致した場合
//         if (strcmp(name, input) == 0) {
//             stock -= 1;          // 在庫を1減らす

//             // 在庫が0未満にならないようにする
//             if (stock < 0) {
//                 stock = 0;
//             }
//         }

//         // 更新したデータを一時ファイルに書き込む
//         fprintf(tmp, "%s,%d\n", name, stock);
//     }

//     // ファイルを閉じる
//     fclose(fp);
//     fclose(tmp);

//     // 元のファイルを削除
//     remove("stock.txt");

//     // 一時ファイルを元ファイル名に変更（上書き）
//     rename("stock_tmp.txt", "stock.txt");

//     printf("Stock updated.\n");

//     return 0;
// }