/**
 * @file move.c
 * @brief Az alapvető lépés és pozíció kezelés.
 * @details Ebben a fájlban találhatóak a position műveleteket végző függvények,
 * valamint a lépéseket végrehajtó függvének
 */
#include "move.h"
#include "board.h"
#include "movegenerator.h"



/**
 * @brief position gyárfüggvény.
 * @details A position típusú változók egyszerűbb
 * @param file  (0-7)
 * @param row  (0-7)
 * @return position 
 */
position p_createpos(int file, int row){ return (position){.file = file, .row = row};}

/**
 * @brief A két position egyenlő?
 * @param pos1
 * @param pos2
 * @return bool (pos1 == pos2) 
 */
int p_isequal(position pos1, position pos2){ return (pos1.row == pos2.row && pos1.file == pos2.file);}

/**
 * @brief Összead 2 positiont.
 * @param pos1 
 * @param pos2 
 * @return position (pos1 + pos2) 
 */
position p_add(position pos1, position pos2){ return p_createpos(pos1.file + pos2.file,pos1.row + pos2.row);}

/**
 * @brief Egész szám és position szorzata.
 * @param pos
 * @param i
 * @return position (a * b)
 */
position p_times(position pos, int i){ return p_createpos(pos.file * i, pos.row * i);}



/**
 * @brief move gyárfüggvény.
 * @details A move gyárfüggvény egyszerűbbé teszi a move típusú változók létrehozását.
 * @param from
 * @param to
 * @param pp A promotion-bábu típusa ha van, egyébként EMPTY.
 * @return move 
 */
move m_createmove(position from, position to, piece_t pp)
{
    move mv;
    mv.from = from;
    mv.to = to;
    mv.promotion_piece = pp;
    return mv;
}


/**
 * @brief Egy normál lépés megtétele a táblán.
 * @details Ezek a lépések nem igényelnek különleges kezelést.
 * @param pbrd 
 * @param mv 
 */
static void play_normal_move(board *pbrd, move mv)
{
    /*normál lépés*/
    /* A lépés meglépése a táblán */
    piece pc;
    (void)b_getpiece(*pbrd, mv.from, &pc);
    /* A bábu adatainak frissítése */
    pc.hasMoved = 1;
    (void)b_setpiece(pbrd, mv.to, pc);
    (void)b_setpiece(pbrd, mv.from, b_create_empty_piece());
}

/**
 * @brief A rövidsánc végrehajtása az adott táblán.
 *  @details A hosszúsánc során a király 2 lépéssel közelebbi oldalon található bástya felé lép,
 * majd az a bástya a másik oldalára kerül. Ennek is különleges a kezelési igénye. https://en.wikipedia.org/wiki/Castling#Rules
 * @param pbrd 
 * @param mv 
 */
static void play_shortcstl(board *pbrd, move mv)
{
    /*rövidsánc*/
    play_normal_move(pbrd, mv);

    /* Az alrébblépés "vektorának" meghatározása a bástyánál */
    position rookPos = p_add(mv.to, RIGHT);
    position rookToPos = p_add(mv.from, RIGHT);

    play_normal_move(pbrd, m_createmove(rookPos, rookToPos, EMPTY));
}

/**
 * @brief A hosszúsánc végrehajtása adott táblán
 * @details A hosszúsánc során a király 2 lépéssel a távolabbi oldalon található bástya felé lép,
 * majd az a bástya a másik oldalára kerül. Ennek is különleges a kezelési igénye. https://en.wikipedia.org/wiki/Castling#Rules
 * @param pbrd 
 * @param mv 
 */
static void play_longcstl(board *pbrd, move mv)
{
    /* király léptetése */
    play_normal_move(pbrd, mv);
    
    /* bástya lépésének meghatározása */
    position rookPos = p_add(mv.to, p_times(LEFT, 2));
    position rookToPos = p_add(mv.to, RIGHT);

    /*bástya léptetése*/
    play_normal_move(pbrd, m_createmove(rookPos, rookToPos, EMPTY));
}

/**
 * @brief A promotion lépés kezelése az adott táblán.
 * @details A promotion lépésnél, vagy átváltozásnál a belépő gyalog egy tetszőleges tisztté változik át, ami szintén
 * különleges kezelést igényel. https://en.wikipedia.org/wiki/Pawn_(chess)#Promotion
 * @param pbrd 
 * @param mv 
 */
static void play_pp_move(board *pbrd, move mv)
{
    /* A szín megtalálásához*/
    piece pc;
    (void)b_getpiece(*pbrd, mv.from, &pc);
    /* mögötti mező törlése */
    (void)b_setpiece(pbrd, mv.from, b_create_empty_piece());
    /* új tiszt lerakása */
    (void)b_setpiece(pbrd, mv.to, b_create_piece(mv.promotion_piece, pc.col, 1));
}

/**
 * @brief A dupla gyaloglépés kezelése az adott táblán.
 * @details A dupla gyaloglépés során létrejön egy olyan postition a táblán, ahova az ellenfél egyik gyalogja
 * En-passant tud ütni. Ezt a positiont a táblán tárolni kell pontosan 1 lépésig,
 * amég az ütési lehetőség fennáll az ellenfélnek. https://en.wikipedia.org/wiki/Pawn_(chess)#Placement_and_movement
 * @param pbrd 
 * @param mv 
 */
static void play_dp_move(board *pbrd, move mv)
{
    /*duplagyalog*/
    piece pc;
    (void)b_getpiece(*pbrd, mv.from, &pc);
    
    /* hátrairány meghatározása */
    position back;
    if(pc.col == WHITE)back = DOWN;
    else back = UP;

    play_normal_move(pbrd, mv);
    /* en passant pozíció meghatározása */
    pbrd->ep_pos = p_add(mv.to, back);
}

/**
 * @brief Az en-passant lépés kezelése, adott táblán.
 * @details Az En-Passant lépés azért igényel különleges kezelést, mert nem az ütő
 * bábu nem a leütött bábu helyét eszi át, hanem az a mögötti mezőre kerül. https://en.wikipedia.org/wiki/Pawn_(chess)#En_passant
 * @param pbrd
 * @param mv 
 */
static void play_ep_move(board *pbrd, move mv)
{
    /* En passant */
    piece pc;
    (void)b_getpiece(*pbrd, mv.from, &pc);

    /* A visszafele irány meghatározása */
    position back;
    if(pc.col == WHITE)back = DOWN;
    else back = UP;

    /* A lépés megtétele*/
    play_normal_move(pbrd, mv);
    
    /* A leütött gyalog törlése */
    position deletePos = p_add(mv.to, back);
    (void)b_setpiece(pbrd,deletePos, b_create_empty_piece());
}   

 
/**
 * @brief Egy a program által generált lépést oszt szét a segédfüggvényeknek. Amik aztán a lépés típusától függően 
 * végrehajtják a lépést. Így tudja a program kezelni a különleges lépéseket.
 * @param pbrd 
 * @param gmv 
 */
static void m_play_generatedmove(board *pbrd, generated_move gmv)
{
    /*A már szabályosa generált lépések közül kiválasztott megtétele, nem szükséges
    szabályellenőrzés*/
    switch(gmv.type)
    {
        case PP:    play_pp_move(pbrd, gmv.move);       break;
        case DP:    play_dp_move(pbrd, gmv.move);       break;
        case CSTL_L:play_longcstl(pbrd, gmv.move);   break;
        case CSTL_S:play_shortcstl(pbrd, gmv.move);   break;
        case EP:    play_ep_move(pbrd, gmv.move);       break;
        default:    play_normal_move(pbrd, gmv.move);
    }
    if(gmv.type != DP) pbrd->ep_pos = NOPOS;
}


/**
 * @brief A default lépéshvégrehajtó függvény
 * @details A függvény a beolvasott lépés-prototípus alapján legenáráltattja az összes
 * lehetséges lépést, majd meghatározza hogy megtalálható-e ezek között a prototípus, ha igen,
 * akkor a lépést megteszi.
 * @param pbrd 
 * @param mv 
 * @return SUCCESS vagy ERROR attól függően hogy sikerült-e
 */
int m_play_move(board *pbrd, move mv)
{
    /* Ha nincs a táblán a kezdő vagy végpozíció kidobjuk */
    if(!b_ison(mv.from) || !b_ison(mv.from))return ERROR;

    /* Azért 27 hosszú a töb, mert 27 a vezér összes lépésése üres táblán
    ennél csak kevesebb lépése lehet egy bábunak*/
    generated_move list[MVL_MAX] = {0};
    int size = 0;

    /* Ha nem sikerül a keresés lépjünk ki ERROR-al*/
    if(mg_generate_all_moves(*pbrd, mv, list, &size) == ERROR) return ERROR;

    /* Keressük meg hogy a lépés kezdőpozíciójából generált lépések között
    van-e olyan aminek a véghelyzete megegyezik a megadott lépés véghelyzetével. 
    Ha igen, hajtsuk végre ezt a generált lépést.*/
    for(int i = 0; i < size; i++)
    {
        if(p_isequal(list[i].move.to, mv.to))
        {
            board testboard = *pbrd;
            m_play_generatedmove(&testboard, list[i]);
            if(b_is_attacked(testboard, b_get_king_pos(testboard, pbrd->side), pbrd->side))return ERROR;
            m_play_generatedmove(pbrd, list[i]);
            pbrd->side = b_opponent_col(pbrd->side);
            return SUCCESS;
        }
    }
    /* Ha valahogyan ide jutunk, biztosan rossz valami*/
    return ERROR;
}