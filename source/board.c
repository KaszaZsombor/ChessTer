#include "types.h"
#include "move.h"

/* Piece gyárfüggvény */
piece b_create_piece(piece_t type, color_t col, int hasMoved)
{
    return (piece){.col = col, .hasMoved = hasMoved, .tpye = type};
}

piece b_create_empty_piece()
{
    return (piece){.col = NONE, .hasMoved = 0, .tpye = EMPTY};
}

/* Visszaad egy memóriaszemét nélküli táblát, ami teljesen üres*/
board b_create()
{
    board brd = 
    {
        .pcs = {0},
        .ep_pos = NOPOS,
        .side = NONE,
    };
    return brd;
}

/* A táblán van-e?*/
int b_ison(position pos)
{
    return !(pos.file > 7 || pos.file < 0 || pos.row > 7 || pos.row < 0);
}

/* Egy bábu lekérése a tábláról, ha helytelen a pozÍció az értéken nem váltotat*/
int b_getpiece(board *pbrd, position pos, piece *pcs)
{
    if(!b_ison(pos))return ERROR;
    *pcs = pbrd->pcs[pos.file][pos.row];
    return SUCCESS;
}

/* Egy bábu egy tábálra helyezése, visszatérési érték magyarázat a TYPES.H fájlban */
int b_setpiece(board *pbrd, position pos, piece pcs)
{
    /* Ha a keresett pozíció nincs a táblán*/
    if(!b_ison(pos))return ERROR;
    pbrd->pcs[pos.file][pos.row] = pcs;
    return SUCCESS;
}

/* Az ellenfél színét adja vissza */
color_t opponent_col(color_t color)
{
    if(color == WHITE)return BLACK;
    if(color == BLACK)return WHITE;
    else return NONE;
}

/* A táblát alaphelyzetbe állítja */
void b_setdefault(board *pbrd)
{
    pbrd->ep_pos = NOPOS;
    pbrd->side = WHITE;
    /* A gyalogokat rakjuk a táblára*/
    for(int i = 0; i < 8; i++)
    {
        (void)b_setpiece(pbrd, p_createpos(i, 1), b_create_piece(PAWN, BLACK, 0));
        (void)b_setpiece(pbrd, p_createpos(i, 6), b_create_piece(PAWN, WHITE, 0));
    }
    /*Tisztek felállítása az enum sorrendjében, az elsőt kihagyva*/
    /*Balról jobbra amég tudunk, majd a maradékot jobbról balra*/
    for(int i = 0; i < 6; i++)
    {
        (void)b_setpiece(pbrd, p_createpos(i, 0), b_create_piece(i+2, BLACK, 0));
        (void)b_setpiece(pbrd, p_createpos(i ,7), b_create_piece(i+2, WHITE, 0));
    }
    for(int i = 0; i < 3; i++)
    {
        (void)b_setpiece(pbrd, p_createpos(7-i, 0), b_create_piece(i+2, BLACK, 0));
        (void)b_setpiece(pbrd, p_createpos(7-i, 7), b_create_piece(i+2, WHITE, 0));
    }
}