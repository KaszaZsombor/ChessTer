#include <stdio.h>
#include "board.h"
#include "types.h"

int main(void){
    board main_board = b_create();

    for(int i = 0; i< 8; i++){
        for(int j = 0; j < 8; j++){
            printf(" %d%d ", main_board.pcs[j][i], main_board.pcs_col[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    b_setdefault(&main_board);

    for(int i = 0; i< 8; i++){
        for(int j = 0; j < 8; j++){
            printf(" %d%d ", main_board.pcs[j][i], main_board.pcs_col[j][i]);
        }
        printf("\n");
    }

    return 0;
}