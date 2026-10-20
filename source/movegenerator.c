#include "movegenerator.h"
#include "move.h"
#include "board.h"




generated_move create_generatedmove(position from, position to, piece_t ppt ,move_t type)
{
    return (generated_move){.move.from = from, .move.to = to, .move.promotion_piece = ppt , .type = type};
}

/* MOVEGENERATOR FÜGGVÉNYEK*/

/* Visszatérési értéke és paraméterei egyeznek a generate_all_moves függvényével, kivéve a piece
paraméter. Azt ott kiszámoltuk, szóval csak átadjui, hogy ne kelljen mégegyszer*/
int generate_all_moves_RBQ(board brd, position from, piece pcs, generated_move *list, int *size)
{
    int count = 0;
    position dirs[8] = {UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT};
    /* a bástya-futó határ*/
    int max = 8;
    int min = 0;

    /* hogy változtassunk a különböző bábukhoz*/
    if(pcs.type == ROOK)max = 4;
    else if( pcs.type == BISHOP)min = 4;

    /* elindulunk az összes lehetséges irányba*/
    for(int i = min; i < max; i++)
    {
        position searchpos = from;
        for(int j = 0; j < 7; j++)/* azért 7-ig megyünk, mert az a maximum 1 irányba a tábla végéig*/
        {
            searchpos = p_add(searchpos, dirs[i]);
            piece foundpcs;
            /* Ha leértünk a tábláról(akkor kapun ók ERROR-t) mennjünk a követkző irányba */
            if(b_getpiece(&brd,searchpos,&foundpcs) == ERROR)break;
            if(foundpcs.type == EMPTY)
            {
                list[count] = create_generatedmove(from, searchpos, EMPTY, NORMAL);
                count++;
                continue; /* folytassuk a kersést az irányban*/
            }
            else if(foundpcs.col != pcs.col)
            {
                /* Ha ellenkező színű nem üres mezőt találunk, az ég jó. De hagyjuk abba a keresést*/
                list[count] = create_generatedmove(from, searchpos, EMPTY, NORMAL);
                count++;
                break;
            }
            else break; /* Ide akkor jutunk ha saját színű bábuhoz értünk. Ezt leütni nem lehet, és mögé sem lehet ütni
            ezért a következő irányra ugrik*/
        }
    }
    *size = count;
    return SUCCESS;
}

/* Generate all moves for the pawns */
int generate_all_moves_P(board brd, position from, piece pcs, generated_move *list, int *size){

}

/* visszaadja sikerült e találni lépést ha igen, akkor beírja a tömbbe és beírja a hosszát*/
/*Ide átmásoljuk a táblát, a generálás során véletlenül sem akarunk az eredetin változtatni
a board, position a lépésehatározáshoz szükséges*/
int mg_generate_all_moves(board brd, position from, generated_move *list, int *size)
{
    piece mvpcs;
    /* biztosan nem lesz error a visszatérése ennek, mert a mg_make_move-bane már leellenőriztük
    hogy a tálán van a position. Ha már itt megvan, átadjuk a lépésgenerálóknak is*/
    (void)b_getpiece(&brd, from, &mvpcs);

    switch(mvpcs.type)
    {
        /*mivel a vezér, bástya és futó hasnloó logika szerint lép, egy függvény
        elég a lépéseik generálásához*/
        case QUEEN:
        case ROOK:
        case BISHOP: return generate_all_moves_RBQ(brd, from, mvpcs, list, size);
        case KING: break;
        case KNIGHT: break;
        case PAWN: break;
        default: return ERROR; /* Csak ha üres mezővel akarnánk lépni*/
    }
    return SUCCESS;
}


