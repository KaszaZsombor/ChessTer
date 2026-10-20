#ifndef UI_H
#define UI_H
#include "types.h"

//typedef enum stlye_t {DEF, DOTS}stlye_t;
/* függvény visszatérés értékei userinputra ELFOGADVA, TÚLCSORDULT, VÉGJEL*/
enum{ACCEPTED, NOT_ACCEPTED ,OVERFLOW, END};

void ui_drawboard(board brd);
int ui_getusrin(int* size, char* msg);

#endif