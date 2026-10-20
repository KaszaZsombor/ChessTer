#include "move.h"
#include "board.h"

/* POSITION FÜGGVÉNYEK AZ EGYSZERŰBB HASZNÁLATHOZ */
/*gyárfüggvény*/
position p_createpos(int file, int row){ return (position){.file = file, .row = row};}
/*egyenlőség ellenőrző*/
int p_isequal(position a, position b){ return (a.row == b.row && a.file == b.file);}
/*összeadó*/
position p_add(position a, position b){ return p_createpos(a.file + b.file,a.row + b.row);}
/*szorzó*/
position p_times(position a, int b){ return p_createpos(a.file * b, a.row * b);}

/* MOVE FÜGGVÉNYEK */
int m_piece_to_p(board *pbrd, position frm, position to)
{
    piece_t mving_pc;
    color_t mving_col;
    if(!b_ison(frm) || !b_ison(to)) return ERROR;
    (void) b_getpiece(pbrd, frm, &mving_pc, &mving_col);
    (void) b_setpiece(pbrd, frm, EMPTY, NONE);
    (void) b_setpiece(pbrd, to, mving_pc, mving_col);
    return SUCCESS;
}