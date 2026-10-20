#ifndef TYPES_H
#define TYPES_H


/*Visszatérési értékek, ezeket adják vissza int-ként a függvények futás után,
jelezve a lefutás sikerességét*/
enum{ERROR = -1, SUCCESS, DRAW, WW, BW};

/* A bábu típusát jelöli, az empty jelölést az üres mező kapja*/
typedef enum piece_t {EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING}piece_t;
/* A bábu színét, vagy a következő játékos színét tárolja. A NONE az üres mezőké*/
typedef enum color_t {NONE, WHITE, BLACK}color_t;
/*A legenerált lépések típusai, ahol a bábucserén kívül más teendő is van*/
typedef enum move_t {NORMAL, DP, PP, CSTL_L, CSTL_S, EP}move_t;

/*Egy táblapozíciót tárol*/
typedef struct
{
    int file;
    int row;
} position;

/* A nincs pozíció jelölése */
#define NOPOS (position) { -1, -1}


/* konstans irányok a lépésgeneráláshoz*/

#define UP (position){0,-1}
#define DOWN (position){0, 1}
#define LEFT (position){-1, 0}
#define RIGHT (position){1, 0}
#define UPRIGHT (position){1,-1}
 /* Az UPLEFT értéke egyezik a NOPOS positionnel, de az UPLEFT egy irányt jelöl nem helyzetet*/
#define UPLEFT (position){-1,-1}
#define DOWNRIGHT (position){1,1}
#define DOWNLEFT (position){-1,-1}

typedef struct
{
    piece_t tpye;
    color_t col;
    int hasMoved;
}piece;

/* Az input ilyen típusban tárolódik*/
typedef struct
{
    position from;
    position to;
    piece_t promotion_piece;
} move;

/*legenerált lépések*/
typedef struct
{
    move move;
    move_t type;
}generated_move;

/*A táblának tárolnia kell a bábukat, azok színét, ki tud sáncolni, van-e valahol en_passant és hogy
ki jön*/
typedef struct
{
    piece pcs[8][8];
    position ep_pos;
    color_t side;
}board;

#endif