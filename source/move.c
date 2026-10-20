#include "move.h"
#include "board.h"
#include "movegenerator.h"

/* POSITION FÜGGVÉNYEK AZ EGYSZERŰBB HASZNÁLATHOZ */
/*gyárfüggvény*/
position p_createpos(int file, int row){ return (position){.file = file, .row = row};}
/*egyenlőség ellenőrző*/
int p_isequal(position a, position b){ return (a.row == b.row && a.file == b.file);}
/*positio összeadó*/
position p_add(position a, position b){ return p_createpos(a.file + b.file,a.row + b.row);}
/*position szorzó*/
position p_times(position a, int b){ return p_createpos(a.file * b, a.row * b);}

/* MOVE FÜGGVÉNYEK */
/*gyárfüggvény*/
move m_createmove(position from, position to, piece_t pp)
{
    move mv;
    mv.from = from;
    mv.to = to;
    mv.promotion_piece = pp;
    return mv;
}

/*normál lépés*/
void make_normal_move(board *pbrd, move mv)
{
    /* A lépés meglépése a táblán */
    piece pc;
    (void)b_getpiece(pbrd, mv.from, &pc);
    /* A bábu adatainak frissítése */
    pc.hasMoved = 1;
    (void)b_setpiece(pbrd, mv.to, pc);
    (void)b_setpiece(pbrd, mv.from, b_create_empty_piece());
    pbrd->ep_pos = NOPOS;
}

/*rövidsánc*/
void make_cstl_s_move(board *pbrd, move mv)
{
    make_normal_move(pbrd, mv);

    /* Az alrébblépés "vektorának" meghatározása a bástyánál */
    position rookPos = p_add(mv.to, RIGHT);
    position rookToPos = p_add(mv.from, RIGHT);
    
    /* en-passant-ot magától visszaállítja NOPOSITIONBE */
    make_normal_move(pbrd, m_createmove(rookPos, rookToPos, EMPTY));
}

/* hosszúsánc */
void make_cstl_l_move(board *pbrd, move mv)
{
    make_normal_move(pbrd, mv);
    
    /* bástya lépésének meghatározása */
    position rookPos = p_add(mv.to, p_times(LEFT, 2));
    position rookToPos = p_add(mv.to, RIGHT);

    /* en passant pozíció automatikus törlése */
    make_normal_move(pbrd, m_createmove(rookPos, rookToPos, EMPTY));
}

/*promotion*/
void make_pp_move(board *pbrd, move mv)
{
    /* A szín megtalálásához*/
    piece pc;
    (void)b_getpiece(pbrd, mv.from, &pc);
    /* mögötti mező törlése */
    (void)b_setpiece(pbrd, mv.from, b_create_empty_piece());
    /* új tiszt lerakása */
    (void)b_setpiece(pbrd, mv.to, b_create_piece(mv.promotion_piece, pc.col, 1));
    pbrd->ep_pos = NOPOS;
}

/*duplagyalog*/
void make_dp_move(board *pbrd, move mv)
{
    piece pc;
    (void)b_getpiece(pbrd, mv.from, &pc);
    
    /* hátrairány meghatározása */
    position back;
    if(pc.col == WHITE)back = DOWN;
    else back = UP;

    make_normal_move(pbrd, mv);
    /* en passant pozíció meghatározása */
    pbrd->ep_pos = p_add(mv.to, back);
}

/* En passant */
void make_ep_move(board *pbrd, move mv)
{
    piece pc;
    (void)b_getpiece(pbrd, mv.from, &pc);

    position back;
    if(pc.col == WHITE)back = DOWN;
    else back = UP;

    /*TÖRLI AZ EN-PASSANT MEZŐT */
    make_normal_move(pbrd, mv);

    position deletePos = p_add(mv.to, back);
    (void)b_setpiece(pbrd,deletePos, b_create_empty_piece());
}   

/*A már szabályosa generált lépések közül kiválasztott megtétele, nem szükséges
szabályellenőrzés*/
void m_make_generatedmove(board *pbrd, generated_move gmv)
{
    switch(gmv.type)
    {
        case PP:    make_pp_move(pbrd, gmv.move);       break;
        case DP:    make_dp_move(pbrd, gmv.move);       break;
        case CSTL_L:make_cstl_l_move(pbrd, gmv.move);   break;
        case CSTL_S:make_cstl_s_move(pbrd, gmv.move);   break;
        case EP:    make_ep_move(pbrd, gmv.move);       break;
        default:    make_normal_move(pbrd, gmv.move);
    }
}


int m_make_move(board *pbrd, move mv)
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
            m_make_generatedmove(pbrd, list[i]);
            return SUCCESS;
        }
    }
    /* Ha valahogyan ide jutunk, biztosan rossz valami*/
    return ERROR;
}