#ifndef TYPES_H
#define TYPES_H

#define MVL_MAX 27

/*Visszatérési értékek, ezeket adják vissza int-ként a függvények futás után,
jelezve a lefutás sikerességét*/
enum{ERROR = -1, SUCCESS};

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
#define NOPOS (position) { .file = -1, .row = -1}


/* konstans irányok a lépésgeneráláshoz*/

#define UP (position)     {.file = 0, .row =-1}
#define DOWN (position)   {.file = 0, .row = 1}
#define LEFT (position)   {.file =-1, .row = 0}
#define RIGHT (position)  {.file = 1, .row = 0}
#define UPRIGHT (position){.file = 1, .row =-1}
 /* Az UPLEFT értéke egyezik a NOPOS positionnel, de az UPLEFT egy irányt jelöl nem helyzetet*/
#define UPLEFT (position)   {.file =-1, .row =-1}
#define DOWNRIGHT (position){.file = 1, .row = 1}
#define DOWNLEFT (position) {.file =-1, .row =-1}

typedef struct
{
    piece_t type;
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