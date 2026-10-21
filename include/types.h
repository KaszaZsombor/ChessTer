/**
 * @file types.h
 * @brief Alapvető típusokat és globális konstansokat határozza meg.
 */

#ifndef TYPES_H
#define TYPES_H

/**
 * @brief A maximum mennyiségű lépés ami generálódhat
 * @details Ez a vezér lépéseinek száma, egy üres táblán.
 */
#define MVL_MAX 27


/**
 * @brief A függvények default visszatérési értékei
 */
enum{ERROR = -1, SUCCESS};

/**
 * @brief A bábutípusokat határozza meg
 * @details Az EMPTY jelöli a nincs bábut.
 */
typedef enum piece_t {EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING}piece_t;

/**
 * @brief A színeket határozza meg
 * @details A NONE jelöli a nincs színt.
 */
typedef enum color_t {NONE, WHITE, BLACK}color_t;



/**
 * @brief A lépéstípusokat tárolja 
 * @details A lépéstípusok tárolása lehetővé teszi a szétválogatásukat, és az egyes típusok különböző kezelési módját
 */
typedef enum move_t {NORMAL, DP, PP, CSTL_L, CSTL_S, EP}move_t;

/**
 * @brief A position a 2 egész számot tárol
 * @details Ezek jelölhetnek egy táblapozíciót vagy irányt is.
 */
typedef struct
{
    int file;
    int row;
} position;

/* A nincs pozíció jelölése */
/**
 * @brief A nincs position jelölése
 */
#define NOPOS (position) { .file = -1, .row = -1}


/* konstans irányok a lépésgeneráláshoz*/
/**
 * @brief Konstans fel irány.
 */
#define UP (position)     {.file = 0, .row =-1}
/**
 * @brief Konstans le irány.
 */
#define DOWN (position)   {.file = 0, .row = 1}
/**
 * @brief Konstans balra irány.
 */
#define LEFT (position)   {.file =-1, .row = 0}
/**
 * @brief Konstans jobbra irány.
 */
#define RIGHT (position)  {.file = 1, .row = 0}
/**
 * @brief Konstans jobbra-fel irány.
 */
#define UPRIGHT (position){.file = 1, .row =-1}
/**
 * @brief Konstans balra-fel irány.
 */
#define UPLEFT (position)   {.file =-1, .row =-1}
/**
 * @brief Konstans jobbra-le irány.
 */
#define DOWNRIGHT (position){.file = 1, .row = 1}
/**
 * @brief Konstans balra-le irány.
 */
#define DOWNLEFT (position) {.file =-1, .row = 1}

/**
 * @brief A bábu alapvető adatait tárolja: szín, típus, lépett-e
 * 
 */
typedef struct
{
    piece_t type;
    color_t col;
    int hasMoved;
}piece;

/**
 * @brief A lépések prototípusát tárolja, ez alapján választ a lehetséges lépések közül
 * 
 */
typedef struct 
{
    position from;
    position to;
    piece_t promotion_piece;
} move;

/**
 * @brief Ez a legenerált lépéseket tárolja
 * @details A generált lépések biztosan szabályosak, így ezek közül választva csak szabályos lépést tudunk lépni.
 */

typedef struct
{
    move move;
    move_t type;
}generated_move;

/**
 * @brief A játék alapvető adatai tárolja
 * @details A board tárol egy 8x8-as táblát, ami a bábukat tartalmazza. Tartalmaz még egy en-passant positiont és
 * a lépő oldal színét.
 */
typedef struct
{
    piece pcs[8][8];
    position ep_pos;
    color_t side;
}board;

#endif