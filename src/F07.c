#include <stdio.h>
#include <string.h>

#define PRODUCT_NAME_MAX_LENGTH 100

// CSVから商品情報を読み込み、特定の製品を新製品に更新
void change_product(const char* csv_file, const char* old_product, const char* new_product, const int* new_product_price, const char* product_id) {
    FILE *file = fopen(csv_file, "w");// 書き込みモードでファイルを開く
    const char* ID = product_id;
    if (file == NULL){
        printf("ファイルが開けませんでした");
        return 1;
    }
    char line[256];
    while( fgets(line, sizeof(line), file) != NULL){
        char product_name[PRODUCT_NAME_MAX_LENGTH];
        if(sscanf(line, "%s,%99[^,],%*d,%d", product_name) == 1){//"%99[^,]"は、カンマまでの文字列を読み取るフォーマット指定子
            if(strcmp(product_name, old_product) == 0){
                fseek(file, -strlen(line), SEEK_CUR); // 行の先頭に戻る SEEK_CUR=現在のファイル位置
                fprintf(file, "%s,%s,%d,%d", ID, new_product, *new_product_price, 50); // 新製品名と新製品の価格を書き込む
                break;
            }
        }
    }
    printf("製品の変更が完了しました\n");
    printf("変更後の製品: %s, 価格: %d\n", new_product, *new_product_price);
    fclose(file);

    return 0;
}

// CSVから商品を読み込み、全製品の商品在庫数を50に更新する関数
void update_stock(const char* csv_file){
    FILE *file = fopen(csv_file, "w");// 書き込みモードでファイルを開く
    if (file == NULL){
        printf("ファイルが開けませんでした");
        return 1;
    }
    // 在庫数を50に更新する処理をここに追加
    char line[256];
    while( fgets(line, sizeof(line), file) != NULL){
        char product_name[PRODUCT_NAME_MAX_LENGTH];
        int price;
        char ID[10];//例:D01
        if(sscanf(line, "%s,%99[^,],%*d,%d", ID,product_name, &price) == 2){//"%99[^,],%*d,%d"は、カンマまでの文字列を読み取るフォーマット指定子
            fseek(file, -strlen(line), SEEK_CUR); // 行の先頭に戻る SEEK_CUR=現在のファイル位置
            fprintf(file, "%s,%s,%d,%d\n", ID, product_name, price, 50); // 製品名と価格を書き込む
        }
    }
    fclose(file);

    return 0;
}