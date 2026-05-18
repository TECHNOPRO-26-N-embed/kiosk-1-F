#include <stdio.h>
#include "global.c"
#include "F01.c"

void select_drink(MONO mono[]){
    int drink_choice=0;
    printf("飲み物のIDを選択してください。\n");
    if(scanf("%d", &drink_choice)!=1)
        return;

    if(drink_choice<1 || drink_choice>50){
        printf("無効な選択です。\n");
        return;
    }

    MONO selected_drink = mono[drink_choice-1];
    printf("選択された飲み物: %s(%d円)\n", selected_drink.name, selected_drink.price);
    if (total_money>=selected_drink.price&& selected_drink.stock>0){
        printf("購入が成功しました。\n");
    }else if(total_money<selected_drink.price){
        printf("申し訳ありませんが、預り金が不足しています。\n");
        return;
        }else if(selected_drink.stock <= 0){
        printf("申し訳ありませんが、選択された飲み物は在庫切れです。\n");
        return;
    }


}