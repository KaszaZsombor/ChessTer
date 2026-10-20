#include "types.h"

/* Visszaad egy memóriaszemét nélküli táblát, ami teljesen üres*/
board b_create(){
    board e_board = {
        .pcs = {EMPTY},
        .pcs_col = {NONE},
        .ep_pos = NOPOS,
        .side = NONE,
        .cstl_rigths_b = {0,0},
        .cstl_rigths_w = {0,0},
    };return e_board;
}

/* Egy bábu lekérése a tábláról, ha helytelen a pozÍció az értéken nem váltotat*/
int b_getpiece(board *pboard, position pos, piece_t *t_out, color_t *col_out){
    if(pos.file > 7 || pos.file < 0 || pos.row > 7 || pos.row < 0)return ERROR;
    *t_out = pboard->pcs[pos.file][pos.row];
    *col_out = pboard->pcs_col[pos.file][pos.row];
    return SUCCESS;
}

/* Egy bábu egy tábálra helyezése, visszatérési érték magyarázat a TYPES.H fájlban */
int b_setpiece(board *pboard, position pos, piece_t pcst, color_t col){
    /* Ha a keresett pozíció nincs a táblán*/
    if(pos.file > 7 || pos.file < 0 || pos.row > 7 || pos.row < 0)return ERROR;
    pboard->pcs[pos.file][pos.row] = pcst;
    pboard->pcs_col[pos.file][pos.row] = col;
    return SUCCESS;
}

/* A táblát alaphelyzetbe állítja */
void b_setdefault(board *pboard){
    for(int i = 0; i < 2; i++){
        pboard->cstl_rigths_b[i] = 1;
        pboard->cstl_rigths_w[i] = 1;
    }
    pboard->ep_pos = NOPOS;
    pboard->side = WHITE;
    /* A gyalogokat rakjuk a táblára*/
    for(int i = 0; i < 8; i++){
        (void)b_setpiece(pboard,(position){i,1}, PAWN, BLACK);
        (void)b_setpiece(pboard,(position){i,6}, PAWN, WHITE);
    }
    /*Tisztek felállítása az enum sorrendjében, az elsőt kihagyva*/
    /*Balról jobbra amég tudunk, majd a maradékot jobbról balra*/
    for(int i = 0; i < 6; i++){
        (void)b_setpiece(pboard, (position){i, 0}, i+2, BLACK);
        (void)b_setpiece(pboard, (position){i, 7}, i+2, WHITE);
    }
    for(int i = 0; i < 3; i++){
        (void)b_setpiece(pboard, (position){7-i, 0}, i+2, BLACK);
        (void)b_setpiece(pboard, (position){7-i, 7}, i+2, WHITE);
    }
}