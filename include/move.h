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
move m_createmove(position from, position to, piece_t pp);
void m_make_generatedmove(board *pbrd, generated_move gmv);
move m_createmove(position from, position to, piece_t pp);
int m_make_move(board *pbrd, move mv);

#endif