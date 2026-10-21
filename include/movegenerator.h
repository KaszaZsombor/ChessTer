#ifndef MOVE_GEN_H
#define MOVE_GEN_H
#include "types.h"

int mg_generate_all_moves(board brd, move mv, generated_move *list, int *size);
void mg_generate_all_moves_RBQ(board brd, position from, piece pcs, generated_move *list, int *size);
void mg_generate_all_moves_N(board brd, position from, piece pcs, generated_move *list, int *size);

#endif