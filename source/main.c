#include <stdio.h>
#include "board.h"
#include "types.h"
#include "ui.h"

int main(void){
    board main_brd = b_create();
    ui_drawboard(main_brd);
    b_setdefault(&main_brd);
    ui_drawboard(main_brd);
    return 0;
}