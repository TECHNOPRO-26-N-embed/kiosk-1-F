#include <stdio.h>

typedef struct MONO{
    int id;
    char name[256];
} MONO;

void drink(void){
    int count =0;
    MONO data[50] = {
        {1, "A"},
        {2, "B"},
        {3, "C"},
        {4, "D"},
        {5, "E"},
        {6, "F"},
        {7, "G"},
        {8, "H"},
        {9, "I"},
        {10, "J"},
        {11, "K"},
        {12, "L"},
        {13, "M"},
        {14, "N"},
        {15, "O"},
        {16, "P"},
        {17, "Q"},
        {18, "R"},
        {19, "S"},
        {20, "T"},
        {21, "U"},
        {22, "V"},
        {23, "W"},
        {24, "X"},
        {25, "Y"},
        {26, "Z"},
        {27, "AA"},
        {28, "AB"},
        {29, "AC"},
        {30, "AD"},
        {31, "AE"},
        {32, "AF"},
        {33, "AG"},
        {34, "AH"},
        {35, "AI"},
        {36, "AJ"},
        {37, "AK"},
        {38, "AL"},
        {39, "AM"},
        {40, "AN"},
        {41, "AO"},
        {42, "AP"},
        {43, "AQ"},
        {44, "AR"},
        {45, "AS"},
        {46, "AT"},
        {47, "AU"},
        {48, "AV"},
        {49, "AW"},
        {50, "AX"},
    };
    for(int i = 0; i < 5;i++){
        for(int j = 0; j < 10;j++){
             printf("%d: %s\t", data[count].id, data[count].name);
            count++;
        }
        printf("\n");
    }
}
