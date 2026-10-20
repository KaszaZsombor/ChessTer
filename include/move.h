#ifndef MOVE_H
#define MOVE_H
#include "types.h"
#include "board.h"

/*POSITION*/
position p_createpos(int file, int row);
int p_isequal(position a, position b);
position p_times(position a, int b);
position p_add(position a, position b);

/*MOVE*/

int m_piece_to_p(board *pbrd, position frm, position to);


#endif