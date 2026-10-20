#include <stdio.h>
#include "board.h"
#include "types.h"
#include "move.h"
#include "ui.h"

int main(void){
    board main_brd = b_create();
    ui_drawboard(main_brd);
    b_setdefault(&main_brd);
    ui_drawboard(main_brd);

    int size = 6;
    char buffer[6];

    int result = ui_getusrin(&size, buffer);
    if(result == END)return 0;
    if(result == OVERFLOW)
    {
        printf("OVER\n");
        return 0;
    }
    if(result == NOT_ACCEPTED)
    {
        printf("UNDER\n");
        return 0;
    }
    printf("%s\n", buffer);

    m_piece_to_p(&main_brd, p_createpos(3,1),p_createpos(1,3));
    ui_drawboard(main_brd);

    return 0;
}