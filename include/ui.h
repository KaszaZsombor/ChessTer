#ifndef UI_H
#define UI_H
#include "types.h"

//typedef enum stlye_t {DEF, DOTS}stlye_t;
/* függvény visszatérés értéke VÉGJEL inputra */
enum{END = -999};

void ui_drawboard(board brd);
int ui_getusrin(int* size, char* msg);
int ui_parse_str(move *pmv, char *buff, int size);

#endif