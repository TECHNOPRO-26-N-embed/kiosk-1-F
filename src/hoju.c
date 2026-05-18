#include <stdio.h>
#include <string.h>

#define PRODUCT_NAME_MAX_LENGTH 100

// CSVから商品情報を読み込み、特定の製品を新製品に更新
void change_product(const char* csv_file, const char* old_product, const char* new_product, int* new_product_price) {
    FILE *file = fopen(csv_file, "r+");
    if (file == NULL){
        printf("ファイルが開けませんでした");
        return 1;
    }
    char line[256];
    while( fgets(line, sizeof(line), file) != NULL){
        char product_name[PRODUCT_NAME_MAX_LENGTH];
        if(sscanf(line, "%99[^,]", product_name) == 1){
            if(strcmp(product_name, old_product) == 0){
                fseek(file, -strlen(line), SEEK_CUR); // 行の先頭に戻る
                fprintf(file, "%s,%d", new_product, *new_product_price); // 新製品名と新製品の価格を書き込む
                break; // 更新後、ループを抜ける
            }
        }
    }
}