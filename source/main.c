#include <stdio.h>
#include "board.h"
#include "types.h"
#include "move.h"
#include "ui.h"

int main(void){
    board main_brd = b_create();
    b_setdefault(&main_brd);

    ui_drawboard(main_brd);

    while (1)
    {
        int usrin_result;
        int size = 6;
        char buffer[6];

        usrin_result = ui_getusrin(&size, buffer);
        
        if(usrin_result == END)break;
        else if(usrin_result == ERROR)
        {
            printf("Nem ertelmezett bemenet\n");
            continue;
        }

        move mv;
        if(ui_parse_str(&mv, buffer, size) == ERROR)
        {
            printf("A lepes nincs a tablan\n");
            continue;
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