/**
 * @file main.c
 * @brief A main a game-loop futtatását irányítja
 */

#include <stdio.h>
#include "board.h"
#include "types.h"
#include "move.h"
#include "ui.h"


/**
 * @brief Elindtítja és futtatja a játékot a mecss végéig.
 */
void game_start(void)
{

    board main_brd = b_create();
    b_setdefault(&main_brd);
    ui_drawboard(main_brd);

    while (1)
    {
        if(main_brd.state == WW)
        {
            printf("Vilagos gyozott\n");
            break;
        }
        if(main_brd.state == BW)
        {
            printf("Sotet gyozott\n");
            break;
        }
        if(main_brd.state == DRAW)
        {
            printf("Dontetlen\n");
            break;
        }


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

        if(m_play_move(&main_brd, mv) == ERROR)
        {
            printf("A lepes nem szabalyos\n");
            continue;
        }




        ui_drawboard(main_brd);
    }
}



/**
 * @brief A főprogram, a játékciklus itt fut
 * @return int 
 */
int main(void)
{
    game_start();
    return 0;
}