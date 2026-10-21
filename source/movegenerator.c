/**
 * @file movegenerator.c
 * @brief A lépések generálásának logikája 
 */
#include "movegenerator.h"
#include "move.h"
#include "board.h"



/* generatedmove gyárfüggvény */
generated_move mg_create_generatedmove(position from, position to, piece_t ppt ,move_t type)
{
    return (generated_move){.move.from = from, .move.to = to, .move.promotion_piece = ppt , .type = type};
}

/* MOVEGENERATOR FÜGGVÉNYEK*/

/* Visszatérési értéke és paraméterei egyeznek a generate_all_moves függvényével, kivéve a piece
paraméter. Azt ott kiszámoltuk, szóval csak átadjui, hogy ne kelljen mégegyszer*/
int mg_generate_all_moves_RBQ(board brd, position from, piece pcs, generated_move *list, int *size)
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
            if(b_getpiece(brd, searchpos, &foundpcs) == ERROR)break;
            if(foundpcs.type == EMPTY)
            {
                list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
                count++;
                continue; /* folytassuk a kersést az irányban*/
            }
            else if(foundpcs.col != pcs.col)
            {
                /* Ha ellenkező színű nem üres mezőt találunk, az ég jó. De hagyjuk abba a keresést*/
                list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
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
int mg_generate_all_moves_P(board brd, move mv, piece pcs, generated_move *list, int *size)
{
    position from = mv.from;
    int count = 0;
    position forward;

    /* Előreirány meghatározása */
    if(pcs.col == WHITE) forward = UP;
    else forward = DOWN;

    /* ütésirányok meghatározása */
    position takedirs[2] = {p_add(forward,LEFT), p_add(forward,RIGHT)};

    position searchpos = from;

    /* ütési pozíciók vizsgálata */
    for(int i = 0; i < 2; i++)
    {
        searchpos = from;
        searchpos = p_add(searchpos, takedirs[i]);
        piece foundpcs;
        if(b_getpiece(brd, searchpos, &foundpcs) == ERROR)continue;/* Ha nincs a táblán, nem jó*/
        /*ha ütési helyzetben egy en-passant pozíció van, az csak úgy lehet ha az előző lépésben
        duplalépést léptk mellénk egy gyaloggal, mert minden lépés után törlődik. Ilyenkor lehet en-passant lépni*/
        if(p_isequal(searchpos, brd.ep_pos))
        {
            /* a lista frissítése*/
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, EP);
            count++;
            continue;
        }
        /* ha ellenfél bábu van itt, le lehet ütni*/
        if(foundpcs.col != pcs.col && foundpcs.type != EMPTY)   
        {
            if(searchpos.row != 0 && searchpos.row != 7)
            {
                list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
                count++;
                continue;
            }
            /* csak akkor jutunk ide ha ütve prmotolunk*/
            if(mv.promotion_piece == EMPTY)return ERROR;/* Ha nincs megadva promotion bábu*/
            list[count] = mg_create_generatedmove(from, searchpos, mv.promotion_piece, PP);
            count++;
            continue;
        }
    }

    /* előrelépések vizsgálata */
    searchpos = from;
    searchpos = p_add(searchpos, forward);

    piece foundpcs;
    /*biztosan a táblán van, ha szélső mezőre ér promotion miatt már nem galog*/
    (void)b_getpiece(brd, searchpos, &foundpcs);

    if(foundpcs.type == EMPTY)
    {
        if(searchpos.row == 0 || searchpos.row == 7)
        {
            /* Ez vagy empty vagy helyes, nem kell mást vizsgálni. Az inputról nem fogadunk el
            rossz promotion_piece adatot*/
            if(mv.promotion_piece == EMPTY)return ERROR;
            list[count] = mg_create_generatedmove(from, searchpos, mv.promotion_piece, PP);
            count++;
        }
        else
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
            count++;
        }
        /* dupla előrelépés vizsgálata */
        searchpos = p_add(searchpos, forward);
        (void)b_getpiece(brd, searchpos, &foundpcs);
        if(foundpcs.type == EMPTY && pcs.hasMoved == 0)
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, DP);
            count++;
        }
    }
    *size = count;
    return SUCCESS;
}

/* A király lépéseinek generálása */
int mg_generate_all_moves_K(board brd, position from, piece pcs, generated_move *list, int *size)
{
    int count;
    position directions[8] = { UP ,DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT };
    
    /* NORMÁL LÉPÉSEK - SAKK ELLENŐRZÉSE KÉSŐBB */
    for( int i = 0; i < 8; i++ )
    {
        position searchpos = from;
        searchpos = p_add(searchpos, directions[i]);
        piece foundpcs;
        if(b_getpiece(brd, searchpos, &foundpcs) == ERROR) continue;
        if(foundpcs.col != pcs.col)
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
            count++;
        }
    }

    /* rövidsánc kezelése */
    /*feltételek: ne legyen sakkban, az áthaladt mezők ne legyenek támadva, nem léptek a királlyal, nem léptek a bástyával
    Üres köztük az út*/



    *size = count;
    return SUCCESS;
}


/* Huszárlépések geberálása */
int mg_generate_all_moves_N(board brd, position from, piece pcs, generated_move *list, int *size)
{
    int count = 0;
    /* A huszár lehetséges relatív lépései*/
    position directions[8] = {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,-1},{-2,1}};

    for(int i = 0; i < 8; i++)
    {
        position searchpos = from;
        /* összes irány vizsgálata */
        searchpos = p_add(searchpos, directions[i]);
        piece foundpcs;
        if(b_getpiece(brd, searchpos, &foundpcs) == ERROR) continue;
        /* Ha a mező üres vagy ellenfél bábuja áll ott*/
        if(foundpcs.col != pcs.col)
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
            count++;   
        }
    }
    *size = count;
    return SUCCESS;
}
/* visszaadja sikerült e találni lépést ha igen, akkor beírja a tömbbe és beírja a hosszát*/
/*Ide átmásoljuk a táblát, a generálás során véletlenül sem akarunk az eredetin változtatni
a board, position a lépésehatározáshoz szükséges*/

int mg_generate_all_moves(board brd, move mv, generated_move *list, int *size)
{
    piece mvpcs;
    /* biztosan nem lesz error a visszatérése ennek, mert a mg_make_move-bane már leellenőriztük
    hogy a tálán van a position. Ha már itt megvan, átadjuk a lépésgenerálóknak is*/
    (void)b_getpiece(brd, mv.from, &mvpcs);

    switch(mvpcs.type)
    {
        /*mivel a vezér, bástya és futó hasnloó logika szerint lép, egy függvény
        elég a lépéseik generálásához*/
        case QUEEN:
        case ROOK:
        case BISHOP: return mg_generate_all_moves_RBQ(brd, mv.from, mvpcs, list, size);
        case KING: return mg_generate_all_moves_K(brd, mv.from, mvpcs, list, size);
        case KNIGHT: return mg_generate_all_moves_N(brd, mv.from, mvpcs, list, size);
        case PAWN: return mg_generate_all_moves_P(brd, mv, mvpcs, list, size);
        default: return ERROR; /* Csak ha üres mezővel akarnánk lépni*/
    }
    return SUCCESS;
}


