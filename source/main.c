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

    int result = 1;
    while (result != END)
    {
        int size = 6;
        char buffer[6];

        result = ui_getusrin(&size, buffer);
        
        if(result == END)break;
        else if(result == ERROR)continue;;

        move mv;
        result = ui_parse_str(&mv, buffer, size);
        
        if(result == ERROR)continue;

        m_make_generatedmove(&main_brd, (generated_move){.move = mv, .type = NORMAL});

        ui_drawboard(main_brd);
    }
    
    

    return 0;
}