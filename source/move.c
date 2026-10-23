/**
 * @file move.c
 * @brief Az alapvető lépés és pozíció kezelés.
 * @details Ebben a fájlban találhatóak a position műveleteket végző függvények,
 * valamint a lépéseket végrehajtó függvének
 */
#include "move.h"
#include "board.h"
#include "movegenerator.h"
#include <stdio.h>

/**
 * @defgroup pos_func Position függvények
 * @brief A position fügvények egyszerűbbé és lehetővé teszik a position típusú változókkal történő
 * műveleteket.
 * @details A nekünk szükséges position-aritmetika az összeadás, szorzás és egyenlőség vizsgálat. Ezen kívül
 * a gyárfüggvény segítségével könnyen kezelhetővé válik ez a típus.
 */


/**
 * @ingroup pos_func
 * @brief A position gyárfüggvény.
 * @details A position típusú változók egyszerűbb létrehozásáéra.
 * @param file  (0-7)
 * @param row  (0-7)
 * @return position 
 */
position p_createpos(int file, int row){ return (position){.file = file, .row = row};}

/**
 * @ingroup pos_func
 * @brief A két megadott pozíció egyenlőségét vizsgálja.
 * @param pos1
 * @param pos2
 * @return bool (pos1 == pos2) 
 */
int p_isequal(position pos1, position pos2){ return (pos1.row == pos2.row && pos1.file == pos2.file);}

/**
 * @ingroup pos_func
 * @brief Összead 2 pozíciót.
 * @param pos1 
 * @param pos2 
 * @return position (pos1 + pos2) 
 */
position p_add(position pos1, position pos2){ return p_createpos(pos1.file + pos2.file,pos1.row + pos2.row);}

/**
 * @ingroup pos_func
 * @brief Egész szám és pozíció szorzata.
 * @param pos
 * @param i
 * @return position (a * b)
 */
position p_times(position pos, int i){ return p_createpos(pos.file * i, pos.row * i);}

/**
 * @defgroup play_helper A lépés függvényei.
 * @brief Ezek a függvények lépik meg a megadott lépést a megadott táblán.
 * @details A m_play_generatedmove függvény ezeknek a függvényeknek osztja szét a generált lépéseket,
 * azok típusa szerint. Ezeknek a függvényeknek a lépés végrehatásának kezelése a feladatuk,
 * és annak a táblának az információinak frissítése, amin a lépést lépték.
 */


 
/**
 * @ingroup play_helper
 * @brief Lépés gyárfüggvény.
 * @details A lépés gyárfüggvény egyszerűbbé teszi a move típusú változók létrehozását.
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
 * @ingroup play_helper
 * @brief Egy normál lépés megtétele a táblán.
 * @details Ezek a lépések nem igényelnek különleges kezelést.
 * @param pbrd 
 * @param mv 
 */
void play_normal_move(board *pbrd, move mv)
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
 * @ingroup play_helper
 * @brief A rövidsánc végrehajtása az adott táblán.
 *  @details A hosszúsánc során a király 2 lépéssel közelebbi oldalon található bástya felé lép,
 * majd az a bástya a másik oldalára kerül. Ennek is különleges a kezelési igénye. https://en.wikipedia.org/wiki/Castling#Rules
 * @param pbrd 
 * @param mv 
 */
void play_shortcstl(board *pbrd, move mv)
{
    /*rövidsánc*/
    play_normal_move(pbrd, mv);

    /* Az alrébblépés "vektorának" meghatározása a bástyánál */
    position rookPos = p_add(mv.to, RIGHT);
    position rookToPos = p_add(mv.from, RIGHT);

    play_normal_move(pbrd, m_createmove(rookPos, rookToPos, EMPTY));
}

/**
 * @ingroup play_helper
 * @brief A hosszúsánc végrehajtása adott táblán.
 * @details A hosszúsánc során a király 2 lépéssel a távolabbi oldalon található bástya felé lép,
 * majd az a bástya a másik oldalára kerül. Ennek is különleges a kezelési igénye. https://en.wikipedia.org/wiki/Castling#Rules
 * @param pbrd 
 * @param mv 
 */
void play_longcstl(board *pbrd, move mv)
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
 * @ingroup play_helper
 * @brief A promotion lépés kezelése az adott táblán.
 * @details A promotion lépésnél, vagy átváltozásnál a belépő gyalog egy tetszőleges tisztté változik át, ami szintén
 * különleges kezelést igényel. https://en.wikipedia.org/wiki/Pawn_(chess)#Promotion
 * @param pbrd 
 * @param mv 
 */
void play_pp_move(board *pbrd, move mv)
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
 * @ingroup play_helper
 * @brief A dupla gyaloglépés kezelése az adott táblán.
 * @details A dupla gyaloglépés során létrejön egy olyan postition a táblán, ahova az ellenfél egyik gyalogja
 * En-passant tud ütni. Ezt a positiont a táblán tárolni kell pontosan 1 lépésig,
 * amég az ütési lehetőség fennáll az ellenfélnek. https://en.wikipedia.org/wiki/Pawn_(chess)#Placement_and_movement
 * @param pbrd 
 * @param mv 
 */
void play_dp_move(board *pbrd, move mv)
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
 * @ingroup play_helper
 * @brief Az en-passant lépés kezelése, adott táblán.
 * @details Az En-Passant lépés azért igényel különleges kezelést, mert nem az ütő
 * bábu nem a leütött bábu helyét eszi át, hanem az a mögötti mezőre kerül. https://en.wikipedia.org/wiki/Pawn_(chess)#En_passant
 * @param pbrd
 * @param mv 
 */
void play_ep_move(board *pbrd, move mv)
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
 * @ingroup play_helper
 * @brief Egy a program által generált lépést oszt szét a segédfüggvényeknek. Amik aztán a lépés típusától függően 
 * végrehajtják a lépést. Így tudja a program kezelni a különleges lépéseket.
 * @param pbrd 
 * @param gmv 
 */
void m_play_generatedmove(board *pbrd, generated_move gmv)
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
 * @brief Visszaadja, hogy van-e lépése a megadott színnek.
 * @ingroup play_helper
 * @details A has_moves függvény ellenőrzi a szín összes bábuját, ha bármelyiknek van lépése, akkor
 * visszatér 1-el. Egyébként visszatér 0-val.
 * @param brd 
 * @param color 
 * @return bool
 */
int has_moves(board brd, color_t color)
{
    /* Ha a szín NONE akkor nincs lépése*/
    if(color == NONE) return 0;
    /* Végigmegy a tábla összes mezőjén, és kiválasztja a megfelelő színű bábukat */
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            piece pc;
            /* A mezők biztosan a táblán vannak, nem lehet ERROR a getpiece*/
            (void)b_getpiece(brd, p_createpos(j, i), &pc);
            if(pc.col == color)
            {
                /* A megfelelő színű bábura legenerálja az összes lépést */
                generated_move list[MVL_MAX];
                int size = 0;
                /* A végpozíció NOPOS, ugyanis mindegy az értéke a program futásának szempontjából. Az átváltozási
                bábu pedig EMPTY, így is lesz átváltozás lépés, viszont automatikusan vezér lesz az átváltozási bábu.
                A függvény szempontjából ez mindegy, csak arra keres kérdést hogy van-e lépés. Az mindegy hogy mi az a
                lépés.*/
                (void)mg_generate_all_moves(brd, m_createmove(p_createpos(j, i), NOPOS, EMPTY), list, &size);
                for(int k = 0; k < size; k++)
                {
                    /* A lépés akkor helyes, ha a próbatáblán való meglépés után nincs a lépett fél királya sakkban,
                    ezt ugyanúgy ellenőrizi, mint azt az m_play_move függvény teszi.*/
                    board testbrd = brd;
                    m_play_generatedmove(&testbrd, list[k]);
                    if(!b_is_attacked(testbrd, b_get_king_pos(testbrd, color), color))return 1;
                }
            }
        }
    }
    return 0;
}


/**
 * @ingroup play_helper
 * @brief A default lépéshvégrehajtó függvény
 * @details A függvény a beolvasott lépésszándék alapján legenáráltattja az összes
 * lehetséges lépést a szándék kezdeti pozíciójából, majd meghatározza hogy megtalálható-e ezek között a szándék, ha igen,
 * akkor a lépést meglépteti a táblán.
 * @param pbrd 
 * @param mv
 * @bug Nem érzékeli az 50 lépéses szabályt
 * @return SUCCESS vagy ERROR attól függően hogy sikerült-e
 */
int m_play_move(board *pbrd, move mv)
{
    /* Ha nincs a táblán a kezdő vagy végpozíció kilépünk */
    if(!b_ison(mv.from) || !b_ison(mv.to))return ERROR;
    
    /* Ha a tábla nem játék állapotban van kilépünk */
    if(pbrd->state != PLAYING) return ERROR;

    /* Azért 27 hosszú a töb, mert 27 a vezér összes lépésése üres táblán
    ennél csak kevesebb lépése lehet egy bábunak*/
    generated_move list[MVL_MAX] = {0};
    int size = 0;

    /* Ha nem sikerül a keresés lépjünk ki ERROR-al */
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
            
            /* Sakkban van a király? */
            if(b_is_attacked(testboard, b_get_king_pos(testboard, pbrd->side), pbrd->side))return ERROR;
            m_play_generatedmove(pbrd, list[i]);

            pbrd->side = b_opponent_col(pbrd->side);
            
            /* Vége van-e a meccsnek? */
            if(!has_moves(*pbrd, pbrd->side))
            {
                if(b_is_attacked(*pbrd, b_get_king_pos(*pbrd, pbrd->side), pbrd->side))
                {
                    if(pbrd->side == WHITE) pbrd->state = BW;
                    else pbrd->state = WW;
                }
                else pbrd->state = DRAW;
            }

            return SUCCESS;
        }


    }
    /* Ha valahogyan ide jutunk, biztosan rossz valami*/
    return ERROR;
}