#include <stdio.h>
#include "F01.h"


 /// @brief ドリンクの情報を表示する関数
 int Drink(){
    int count =0;
    
    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 10;j++){
             printf("%d: %s\t", data[count].id, data[count].name);
            count++;
        }
        printf("\n");
    }
    return 0;
}
