/**
 * @file movegenerator.c
 * @brief A lépések generálásának logikája 
 */
#include "movegenerator.h"
#include "move.h"
#include "board.h"

/**
 * @defgroup move_gen Lépésgeneráló függvények
 * @brief Ezek a függvények a bábuk lépéseit generálják
 * @details A lépésgeneráló függvények az egyik legfontosabb és központibb része a programnak. Ezek a függvények
 * felelősek a megadott lépés-szándék alapján, az összes lehetséges lépés előállításáért.
 * A legtöbb hiba és bug is itt keletkezett.
 */

/**
 * @ingroup move_gen
 * @brief A generatedmove gyárfüggvény. 
 * @details Egyszerűbbé teszi a generated_move típusú változók létrehozását.
 * @param from 
 * @param to 
 * @param ppt 
 * @param type 
 * @return generated_move 
 */
generated_move mg_create_generatedmove(position from, position to, piece_t ppt ,move_t type)
{
    /* generatedmove gyárfüggvény */
    return (generated_move){.move.from = from, .move.to = to, .move.promotion_piece = ppt , .type = type};
}


/**
 * 
 * @ingroup move_gen
 * @brief Legenerálja az összes szabályos lépést, egy bástyára, futóra vagy vezérre.
 * @details Ez a hárum bábutípus hasonló logika szerint lép. Elindulunk az összes bábu által látható
 * irányba, majd egyesével haladunk a tábla pozícióin az egyes irányokba. Ha a talált mező üres,
 * akkor megfelel és folytatjuk a keresést. Ha a talált bábu ellenséges, az megfelel, vizsont továbbb nem tudunk menni
 * abba az irányba, ezért megyünk a következőre. Ha a talált bábu saját, már az a mező sem jó, és jöhet a következő irány.
 * És ha leérünk a tábláról, akkor is jöhet a következő irány.
 * @param brd 
 * @param from 
 * @param pc 
 * @param list Ebbe a tömbbe másolja a generált lépéseket a függvény.
 * @param size Ebbe az intbe írja bele a visszaadott lista hosszát.
 */
void mg_generate_all_moves_RBQ(board brd, position from, piece pc, generated_move *list, int *size)
{
    /* Visszatérési értéke és paraméterei egyeznek a generate_all_moves függvényével, kivéve a piece
    paraméter. Azt ott kiszámoltuk, szóval csak átadjui, hogy ne kelljen mégegyszer*/
    int count = 0;
    position dirs[8] = {UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT};
    /* a bástya-futó határ*/
    int max = 8;
    int min = 0;

    /* hogy változtassunk a különböző bábukhoz*/
    if(pc.type == ROOK)max = 4;
    else if( pc.type == BISHOP)min = 4;

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
            else if(foundpcs.col != pc.col)
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
}


/**
 * @ingroup move_gen
 * @brief Az összes gyaloglépés generálása egy adott pozícióból.
 * @details A gyloglépések generálása is összetettebb. Ellenőrizni kell az en-passant lehetőségét, a dupla-lépés
 * lehetőségét, az ütéseket. És ezek alapján megfelelő lépéstípusú lépéseket létrehozni.
 * @param brd 
 * @param mv 
 * @param pc 
 * @param list Ebbe a tömbbe másolja a generált lépéseket a függvény.
 * @param size Ebbe az intbe írja bele a visszaadott lista hosszát.
 */
void mg_generate_all_moves_P(board brd, move mv, piece pc, generated_move *list, int *size)
{
    /* Generate all moves for the pawns */
    position from = mv.from;
    int count = 0;
    position forward;

    /* Előreirány meghatározása */
    if(pc.col == WHITE) forward = UP;
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
            /*ne üssünk saját gyalogot en-passant módon*/
            if(pc.col == WHITE && searchpos.row >= 4)continue;
            if(pc.col == BLACK && searchpos.row < 4)continue;
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, EP);
            count++;
            continue;
        }
        /* ha ellenfél bábu van itt, le lehet ütni*/
        if(foundpcs.col != pc.col && foundpcs.type != EMPTY)   
        {
            if(searchpos.row != 0 && searchpos.row != 7)
            {
                list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
                count++;
                continue;
            }
            /* csak akkor jutunk ide ha ütve prmotolunk*/
            if(mv.promotion_piece == EMPTY)mv.promotion_piece = QUEEN;/* Ha nincs megadva promotion bábu*/
            list[count] = mg_create_generatedmove(from, searchpos, mv.promotion_piece, PP);
            count++;
            continue;
        }
    }

    /* előrelépések vizsgálata */
    searchpos = from;
    searchpos = p_add(searchpos, forward);

    piece foundpcs;

    if( b_getpiece(brd, searchpos, &foundpcs) != ERROR && foundpcs.type == EMPTY)
    {
        if(searchpos.row == 0 || searchpos.row == 7)
        {
            /* Ez vagy empty vagy helyes, nem kell mást vizsgálni. Az inputról nem fogadunk el
            rossz promotion_piece adatot*/
            if(mv.promotion_piece == EMPTY)mv.promotion_piece = QUEEN;
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
        if(foundpcs.type == EMPTY && pc.hasMoved == 0)
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, DP);
            count++;
        }
    }
    *size = count;
}


/**
 * @ingroup move_gen
 * @brief Egy adott király tud-e sáncolni rövidre.
 * @details Ez a függvény segít megállapítani a királylépés generáló függvénynek, hogy lehet rövidre sáncolni.
 * Ennek is több különböző eltétele van, amit ellenőrizni kell.
 * @param brd 
 * @param pos
 * @param pc 
 * @return bool 
 */
int canshortcstl(board brd, position pos, piece pc)
{
    /* rövidsánc kezelése */
    /*feltételek: ne legyen sakkban, az áthaladt mezők ne legyenek támadva, nem léptek a királlyal, nem léptek a bástyával
    Üres köztük az út*/
    if(pc.hasMoved)return 0;
    piece foundpcs;
    position rookpos = p_add(pos, p_times(RIGHT, 3));
    if(b_getpiece(brd, rookpos, &foundpcs) == ERROR)return 0;
    if(foundpcs.col != pc.col || foundpcs.type != ROOK || foundpcs.hasMoved)return 0;

    position searchpos = pos;
    for(int i = 0; i < 3; i++)
    {
        searchpos = pos;
        searchpos = p_add(searchpos, p_times(RIGHT, i));
        if(b_getpiece(brd, searchpos, &foundpcs) == ERROR)return 0;
        if( i != 0 && foundpcs.type != EMPTY) return 0;
        if(b_is_attacked(brd, searchpos, pc.col)) return 0;
    }
    return 1;
}


/**
 * @ingroup move_gen
 * @brief Egy adott király tud-e sáncolni hosszúra.
 * @details Ez a függvény segít megállapítani a királylépés generáló függvénynek, hogy lehet hosszúra sáncolni.
 * Ennek is több különböző eltétele van, amit ellenőrizni kell.
 * @param brd 
 * @param pos 
 * @param pc 
 * @return bool 
 */
int canlongcstl(board brd, position from, piece pc)
{
    /* hosszúsánc kezelése */
    /*feltételek: ne legyen sakkban, az áthaladt mezők ne legyenek támadva, nem léptek a királlyal, nem léptek a bástyával
    Üres köztük az út*/
    if(pc.hasMoved)return 0;
    piece foundpcs;
    position rookpos = p_add(from, p_times(LEFT, 4));
    if(b_getpiece(brd, rookpos, &foundpcs) == ERROR)return 0;
    if(foundpcs.col != pc.col || foundpcs.type != ROOK || foundpcs.hasMoved)return 0;

    position searchpos;
    for(int i = 0; i < 4; i++)
    {
        searchpos = from;
        searchpos = p_add(searchpos, p_times(LEFT, i));
        if(b_getpiece(brd, searchpos, &foundpcs) == ERROR)return 0;
        if(i != 0 && foundpcs.type != EMPTY) return 0;
        if(i < 3 && b_is_attacked(brd, searchpos, pc.col)) return 0;
    }
    return 1;
}


/**
 * @ingroup move_gen
 * @brief A király lépéseinek generálása.
 * @details A királynak több különleges lépése van. Ezek a 2 sánc. Ezeknek az ellenőrzését segédfüggvények hatják végre.
 * A függvény nem ellenőrzi, hogy a király lépés után támadott mezőn van-e. Ez azért van, mert nem csak királylépés után
 * kerülhet a király sakkba. A sakkokat egy próbatáblán ellenőrzi, a lépésvégrehajtó függvény.
 * @param brd 
 * @param from 
 * @param pcs 
 * @param list Ebbe a tömbbe másolja a generált lépéseket a függvény.
 * @param size Ebbe az intbe írja bele a visszaadott lista hosszát.
 */
void mg_generate_all_moves_K(board brd, position from, piece pc, generated_move *list, int *size)
{
    int count = 0;
    position directions[8] = { UP ,DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT };
    position searchpos;
    piece foundpcs;
    
    /* NORMÁL LÉPÉSEK - SAKK ELLENŐRZÉSE KÉSŐBB - 
    mive a sakk nem csak a király lépésétől függ hanem rengeteg különböző módon előfordulhat
    (pl felfedett sakk) a lépés megtétele utáni pozícióban lesz ellenőrizve. így itt felesleges ellenőrizni */
    for( int i = 0; i < 8; i++ )
    {
        searchpos = from;
        searchpos = p_add(searchpos, directions[i]);
        if(b_getpiece(brd, searchpos, &foundpcs) == ERROR) continue;
        if(foundpcs.col != pc.col)
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
            count++;
        }
    }

    if(canshortcstl(brd, from, pc))
    {
        list[count] = mg_create_generatedmove(from, p_add(from, p_times(RIGHT, 2)), EMPTY, CSTL_S);
        count++;
    }

    if(canlongcstl(brd, from, pc))
    {
        list[count] = mg_create_generatedmove(from, p_add(from, p_times(LEFT, 2)), EMPTY, CSTL_L);
        count++;
    }

    *size = count;
}


/**
 * @ingroup move_gen
 * @brief A huszár lépéseinek generálása.
 * @details A huszárléppések generálása viszonylag egyszerűbb. Csak a lehetséges veégpozíciók ellenőrzése szükséges,
 * ugyanis a huszár át tudja ugrani a többi bábut.
 * @param brd 
 * @param from
 * @param pc 
 * @param list Ebbe a tömbbe másolja a generált lépéseket a függvény.
 * @param size Ebbe az intbe írja bele a visszaadott lista hosszát. 
 * @return int 
 */
void mg_generate_all_moves_N(board brd, position from, piece pc, generated_move *list, int *size)
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
        if(foundpcs.col != pc.col)
        {
            list[count] = mg_create_generatedmove(from, searchpos, EMPTY, NORMAL);
            count++;   
        }
    }
    *size = count;
}


/**
 * @ingroup move_gen
 * @brief Az érkező lépésprototípust adja a megfelelő lépésgenerátornak, a bábu typusa alapján.
 * @details Ez a fő lépésgeneráló függvény. Az érkező lépés-prototípus alapján legeneráltatja a megfelelő
 * generálófüggvénnyel az összes lehetséges lépést, majd visszaküldi azt a hívónak.
 * @param brd
 * @param mv 
 * @param list Ebbe a tömbbe másolja a generált lépéseket a függvény.
 * @param size Ebbe az intbe írja bele a visszaadott lista hosszát.
 * @return SUCCESS vagy ERROR a sikerességtől függően 
 */
int mg_generate_all_moves(board brd, move mv, generated_move *list, int *size)
{
    /* visszaadja sikerült e találni lépést ha igen, akkor beírja a tömbbe és beírja a hosszát*/
    /*Ide átmásoljuk a táblát, a generálás során véletlenül sem akarunk az eredetin változtatni
    a board, position a lépésehatározáshoz szükséges*/
    piece mvpcs;

    if(b_getpiece(brd, mv.from, &mvpcs) == ERROR){*size = 0; return ERROR;}

    if(mvpcs.col != brd.side)return ERROR;
    switch(mvpcs.type)
    {
        /*mivel a vezér, bástya és futó hasnloó logika szerint lép, egy függvény
        elég a lépéseik generálásához*/
        case QUEEN:
        case ROOK:
        case BISHOP: mg_generate_all_moves_RBQ(brd, mv.from, mvpcs, list, size); break;
        case KING: mg_generate_all_moves_K(brd, mv.from, mvpcs, list, size); break;
        case KNIGHT: mg_generate_all_moves_N(brd, mv.from, mvpcs, list, size); break;
        case PAWN: mg_generate_all_moves_P(brd, mv, mvpcs, list, size); break;
        default: *size = 0; return ERROR; /* Csak ha üres mezővel akarnánk lépni*/
    }
    return SUCCESS;
}
