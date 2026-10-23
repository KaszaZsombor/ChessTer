/**
 * @file board.c
 * @brief A tábla kezelését szolgáló függvények.
 */
#include "types.h"
#include "move.h"
#include "movegenerator.h"

/**
 * @defgroup piece_help Bábutípus függvényei
 * @brief A bábuk kezelését könnyítő függvények.
 */


/**
 * @ingroup piece_help
 * @brief Piece gyárfüggvény adott adatok alapján.
 * @param type  
 * @param col   
 * @param hasMoved  
 * @return piece 
 */
piece b_create_piece(piece_t type, color_t col, int hasMoved)
{
    /* Piece gyárfüggvény */
    return (piece){.col = col, .hasMoved = hasMoved, .type = type};
}


/**
 * @ingroup piece_help
 * @brief Egy üres bábut visszaadó gyárfüggvény.
 * @return piece 
 */
piece b_create_empty_piece(void)
{
    /*empty piece gyárfüggvény*/
    return (piece){.col = NONE, .hasMoved = 0, .type = EMPTY};
}


/**
 * @defgroup board_help A Tábla függvényei
 * @brief A tábla kezelését végző függvények.
 * @details A tábla kezelése minden sakkprogram alapja, a prgoram többi része
 * ezek a függvények segítségével döntenek a feladatuk végrehajtásával kapcsolatban.
 */
 
 

/**
 * @ingroup board_help
 * @brief board gyárfüggvény
 * @return board 
 */
board b_create(void)
{
    /* Visszaad egy memóriaszemét nélküli táblát, ami teljesen üres*/
    board brd = 
    {
        .pcs = {0},
        .ep_pos = NOPOS,
        .side = NONE,
        .state = PLAYING,
    };
    return brd;
}


/**
 * @ingroup board_help
 * @brief Visszaadja, hogy a pozíció a táblán van-e vagy sem.
 * @param pos 
 * @return bool
 */
int b_ison(position pos)
{
    return !(pos.file > 7 || pos.file < 0 || pos.row > 7 || pos.row < 0);
}


/**
 * @ingroup board_help
 * @brief Egy piece lekérése egy tábláról.
 * @param brd 
 * @param pos 
 * @param ppc 
 * @return SUCCESS VAGY ERROR attól függően, hogy a táblán van-e a position. 
 */
int b_getpiece(board brd, position pos, piece *ppc)
{
    /* Egy bábu lekérése a tábláról, ha helytelen a pozÍció az értéken nem váltotat*/
    if(!b_ison(pos))return ERROR;
    *ppc = brd.pcs[pos.file][pos.row];
    return SUCCESS;
}


/**
 * @ingroup board_help
 * @brief Beállítja az adott bábut az adott tábálra.
 * @param pbrd 
 * @param pos 
 * @param pc 
 * @return ERROR vagy SUCCESS, attól függően hogy a pos a táblán van-e.
 */
int b_setpiece(board *pbrd, position pos, piece pc)
{
    /* Egy bábu egy tábálra helyezése, visszatérési érték magyarázat a TYPES.H fájlban */
    /* Ha a keresett pozíció nincs a táblán */
    if(!b_ison(pos))return ERROR;
    pbrd->pcs[pos.file][pos.row] = pc;
    return SUCCESS;
}


/**
 * @ingroup board_help
 * @brief Visszaadja az ellenfél színét.
 * @param color
 * @return color_t az ellemnfél színe.
 */
color_t b_opponent_col(color_t color)
{
    /* Az ellenfél színét adja vissza */
    if(color == WHITE)return BLACK;
    if(color == BLACK)return WHITE;
    else return NONE;
}


/**
 * @ingroup board_help
 * @brief Visszaadja egy táblán a király helyzetét.
 * @param brd 
 * @param col 
 * @return position NOPOS ha nincs a táblán
 */
position b_get_king_pos(board brd, color_t col)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            piece pc = brd.pcs[j][i];
            if(pc.col == col && pc.type == KING)return p_createpos(j,i);
        }
    }
    return NOPOS;
}


/**
 * @ingroup board_help
 * @brief Az adott táblát alaphelyzetbe állítja.
 * @param pbrd 
 */
void b_setdefault(board *pbrd)
{
    /* A táblát alaphelyzetbe állítja */
    pbrd->ep_pos = NOPOS;
    pbrd->side = WHITE;
    pbrd->state = PLAYING;
    /* A gyalogokat rakjuk a táblára*/
    for(int i = 0; i < 8; i++)
    {
        (void)b_setpiece(pbrd, p_createpos(i, 1), b_create_piece(PAWN, BLACK, 0));
        (void)b_setpiece(pbrd, p_createpos(i, 6), b_create_piece(PAWN, WHITE, 0));
    }
    /*Tisztek felállítása az enum sorrendjében, az elsőt kihagyva*/
    /*Balról jobbra amég tudunk, majd a maradékot jobbról balra*/
    for(int i = 0; i < 5; i++)
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


/**
 * @ingroup board_help
 * @brief Megnézi az adott position támadva van-e, egy adott szín által.
 * @param brd 
 * @param pos 
 * @param col   A támadott szín.
 * @return bool
 * @warning A gyalogtámadást nincs tesztelve
 */
int b_is_attacked(board brd, position pos, color_t col)
{
    if(col == NONE) return 0;

    /* megnézi a pozíció támadva van-e, a szín anit támad, NEM a támadó*/
    generated_move rookmove_l[MVL_MAX];
    generated_move bishopmove_l[MVL_MAX];
    generated_move knightmove_l[MVL_MAX];
    generated_move kingmove_l[MVL_MAX];

    /* A visszadatt lépéstömbök hosszai */
    int rookl_size = 0;
    int bishl_size = 0;
    int knightl_size = 0;
    int kingmove_size = 0;

    /* A huszárlépések legenerálása erről a mezőről, a visszakapott move-ok közül
    megvizsgáljuk melyiken található ellenfél bábu, ami ezt a lépést tudná tenni*/
    (void)mg_generate_all_moves_N(brd, pos, b_create_piece(KNIGHT, col, 0), knightmove_l, &knightl_size);
    (void)mg_generate_all_moves_RBQ(brd, pos, b_create_piece(ROOK, col, 0), rookmove_l, &rookl_size);
    (void)mg_generate_all_moves_RBQ(brd, pos, b_create_piece(BISHOP, col, 0), bishopmove_l, &bishl_size);

    /*A királylépéseket egy már hasMoved = 1 királlyal generáltatjuk, hogy csak normál lépéseket kapjunk a vizsgalathoz*/
    (void)mg_generate_all_moves_K(brd, pos, b_create_piece(KING, col, 1), kingmove_l, &kingmove_size);

    /* Megnézzük hogy a generált lépések végpozíciójában van-e olyan bábu ami tudna oda támadni. */
    /*(void) cast a getpiece előtt, biztosan a táblán vannak a kapott pozíciók*/
    piece pc;
    for( int i = 0; i < knightl_size; i++)
    {
        (void)b_getpiece(brd, knightmove_l[i].move.to, &pc);
        if( pc.type == KNIGHT ) return 1; /* EZ EGY BOOL*/
    }
    /* Futós ellenőrzése, vezérsellenőrzés 1 */
    for(int i = 0; i < bishl_size; i++)
    {
        (void)b_getpiece(brd, bishopmove_l[i].move.to, &pc);
        if(pc.type == BISHOP || pc.type == QUEEN)return 1; /* BOOL*/
    }
    /* Bástya ellenőrzése, vezérs ellenőrzés 2*/
    for(int i = 0; i < rookl_size ; i++)
    {
        (void)b_getpiece(brd, rookmove_l[i].move.to, &pc);
        if(pc.type == ROOK || pc.type == QUEEN) return 1; /* BOOL*/
    }
    /* Király ellenőrzése */
    for(int i = 0; i < kingmove_size; i++)
    {
        (void)b_getpiece(brd, kingmove_l[i].move.to, &pc);
        if(pc.type == KING ) return 1;/* BOOL */
    }
    
    
    /* gyalogtámadás ellenőrzése */
    position dirs[2];
    
    /* A lehetséges támadási pozíciók */
    if(col == WHITE){ dirs[0] = UPLEFT; dirs[1] = UPRIGHT;}
    else{dirs[0] = DOWNLEFT; dirs[1] = DOWNRIGHT;}
    
    
    for(int i = 0; i < 2; i++)
    {
        /* Ezeket a lépéseket nem a generálófüggvény generálta, lehetséges hogy nincsenek a táblán*/
        if(b_getpiece(brd, p_add(pos, dirs[i]), &pc) == ERROR)continue;
        if(pc.col != col && pc.type == PAWN) return 1;
    }

    return 0;
}