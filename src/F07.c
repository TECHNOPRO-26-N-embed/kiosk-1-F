#include <stdio.h>
#include <string.h>
#include "global.c"
#include "F01.h"

#define PRODUCT_NAME_MAX_LENGTH 100

// CSVから商品情報を読み込み、特定の製品を新製品に更新
void change_product() {
    // FILE *file = fopen("product.csv", "r+");// 書き込みモードでファイルを開く
    // const char* ID = product_id;
    // if (file == NULL){
    //     printf("ファイルが開けませんでした\n");
    //     return;
    // }
    // char line[256];
    // while( fgets(line, sizeof(line), file) != NULL){
    //     char product_name[PRODUCT_NAME_MAX_LENGTH];
    //     if(sscanf(line, "%s,%99[^,],%*d,%d", product_name) == 1){//"%99[^,]"は、カンマまでの文字列を読み取るフォーマット指定子
    //         if(strcmp(product_name, old_product) == 0){
    //             fseek(file, -strlen(line), SEEK_CUR); // 行の先頭に戻る SEEK_CUR=現在のファイル位置
    //             fprintf(file, "%s,%s,%d,%d", ID, new_product, *new_product_price, 50); // 新製品名と新製品の価格を書き込む
    //             break;
    //         }
    //     }
    // }
    // printf("製品の変更が完了しました\n");
    // printf("変更後の製品: %s, 価格: %d\n", new_product, *new_product_price);
    // fclose(file);
    // F01.hで定義されているdata配列を使用して製品を変更する
    int flag = 0;
    char old_product[PRODUCT_NAME_MAX_LENGTH];
    char new_product[PRODUCT_NAME_MAX_LENGTH];
    int new_product_price;
    do{
        printf("変更前の製品の名前を入力してください: ");
        if (scanf("%s", old_product) != 1) {
            printf("製品名の入力に失敗しました\n");
            while (getchar() != '\n');
            continue;
        }
        printf("変更後の製品の名前を入力してください: ");
        if (scanf("%s", new_product) != 1) {
            printf("製品名の入力に失敗しました\n");
            while (getchar() != '\n');
            continue;
        }
        
        printf("変更後の製品の価格を入力してください: ");
        if (scanf("%d", &new_product_price) != 1) {
            printf("価格の入力に失敗しました\n");
            while (getchar() != '\n');
            continue;
        }
        flag = 1;
    }while(flag == 0);
    for(int i = 0; i < 50; i++){
        if(strcmp(data[i].name, old_product) == 0){
            strncpy(data[i].name, new_product, sizeof(data[i].name)-1);
            data[i].price = new_product_price;
            data[i].stock = 50; // 在庫数を50にリセット
            printf("製品の変更が完了しました\n");
            printf("変更後の製品: %s, 価格: %d\n", data[i].name, data[i].price);
            break;
        }
    }

    return;
}

// CSVから商品を読み込み、全製品の商品在庫数を50に更新する関数
void update_stock(){
    // FILE *file;
    // file = fopen("product.csv", "r");// 書き込みモードでファイルを開く
    // if (file == NULL){
    //     printf("ファイルが開けませんでした\n");
    //     return ;
    // }
    // // 在庫数を50に更新する処理
    // char line[1024];
    // //ヘッダー行の読み飛ばし
    // if (fgets(line, sizeof(line), file) == NULL) {
    //     printf("ファイルの読み取りに失敗しました\n");
    //     fclose(file);
    //     return;
    // }
    // while( fgets(line, sizeof(line), file) != NULL){
    //     char product_name[PRODUCT_NAME_MAX_LENGTH];
    //     int price;
    //     int stock;
    //     char ID[10];//例:D01
    //     if(sscanf(line, "%s,%99[^,],%*d,%d", ID,product_name, &price, &stock) == 0){//"%99[^,],%*d,%d"は、カンマまでの文字列を読み取るフォーマット指定子    
    //         printf("行の読み取りに失敗しました: %s\n", line);
    //         continue;        
    //     }
    //     fseek(file, -strlen(line), SEEK_CUR); // 行の先頭に戻る SEEK_CUR=現在のファイル位置
    //     fprintf(file, "%s,%s,%d,%d\n", ID, product_name, price, 50); // 製品名と価格を書き込む
    //     printf("%sの在庫数を50に更新しました\n", ID);
    // }
    // fclose(file);
    // printf("全製品の在庫数を50に補充しました\n");

    // F01.hで定義されているdata配列を使用して在庫数を50に更新する
    for(int i = 0; i < 50; i++){
        data[i].stock = 50;
    }


    return;
}