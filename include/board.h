#ifndef BOARD_H
#define BOARD_H
#include "types.h"

board b_create();
void b_setdefault(board *pbrd);
color_t b_opponent_col(color_t color);
int b_getpiece(board brd, position pos, piece *pcs);
int b_setpiece(board *pbrd, position pos, piece pcs);
int b_ison(position pos);
piece b_create_piece(piece_t type, color_t col, int hasMoved);
piece b_create_empty_piece();
int b_is_attacked(board brd, position pos, color_t col);
position b_get_king_pos(board brd, color_t col);

#endif