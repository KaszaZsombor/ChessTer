#ifndef BOARD_H
#define BOARD_H
#include "types.h"

/*GYÁR*/
board b_create();
piece b_create_piece(piece_t type, color_t col, int hasMoved);
piece b_create_empty_piece();

/*SET*/
void b_setdefault(board *pbrd);
int b_setpiece(board *pbrd, position pos, piece pcs);

/*BOOl*/
int b_ison(position pos);
int b_is_attacked(board brd, position pos, color_t col);

/*GET*/
int b_getpiece(board brd, position pos, piece *pcs);
color_t b_opponent_col(color_t color);
position b_get_king_pos(board brd, color_t col);


#endif