#include <stdio.h>
#include "board.h"
#include "types.h"
#include "move.h"
#include "ui.h"

int main(void){
    board main_brd = b_create();
    b_setdefault(&main_brd);

    b_setpiece(&main_brd, p_createpos(4,6), b_create_empty_piece());

    ui_drawboard(main_brd);

    int result = 1;
    while (result != END)
    {
        int size = 6;
        char buffer[6];

        result = ui_getusrin(&size, buffer);
        
        if(result == END)break;
        else if(result == ERROR)
        {
            printf("Nem ertelmezett bemenet\n");
            continue;
        }

        move mv;
        if(ui_parse_str(&mv, buffer, size) == ERROR)
        {
            printf("A lepes nincs a tablan\n");
        };

        if(m_make_move(&main_brd, mv) == ERROR)
        {
            printf("A lepes nem szabalyos\n");
            continue;
        }

        ui_drawboard(main_brd);
    }
    return 0;
}