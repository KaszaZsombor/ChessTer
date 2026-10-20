#ifndef BOARD_H
#define BOARD_H
#include "types.h"

board b_create();
void b_setdefault(board *pbrd);
color_t opponent_col(color_t color);
int b_getpiece(board *pbrd, position pos, piece_t *t_out, color_t *col_out);

#endif