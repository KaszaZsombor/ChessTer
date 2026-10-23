/**
 * @file ui.c
 * @brief A felhasználó - program közti információküzlést végzi.
 */
#include <stdio.h>
#include "ui.h"
#include "move.h"


/**
 * @defgroup ui A user interface
 * @brief A program felhasználója és a program sakk-logikája között álló fordító réteg.
 * @details A projektben képességeim szerint megpróbáltam a lehető legjobban szétválasztani a UI-t a program többi részétől.
 * Ennek oka az, ha később grafikus megjelenítést szeretnék készíteni a programhoz, akkor magával a logikával nem kell
 * foglalkoznom. Elég megírni ezt a köztes réteget, a grafikus megjelenítést, majd hozzá lehet kötni ugyanazt a
 * kódot.
 */

/**
 * @ingroup ui
 * @brief A user által beadott szöveg feldolgozása.
 * @param size A feldolgozott szöveg hossza.
 * @param msg A feldolgozott szöveg.
 * @return ERROR, SUCCESS vagy END. ERROR ha sikertelen, SUCCESS ha sikeres, END ha végjelet kapott.
 */
int ui_getusrin(int *size, char *msg)
{
    /* A kimenet a beolvasás sikerét határozza meg,
    a size befele a maximum msg size, kifele a beolvasott size*/
    int i;
    for(i = 0; i < *size; i++)
    {
        if(scanf("%c", &msg[i]) == EOF)return END;
        if(msg[i] == '\n')
        {
            /* Ha megfelelő hosszú szöveget kaptunk*/
            if(i >= 4)
            {
                *size = i + 1;
                return SUCCESS;
            }
            else return ERROR;
        }
    }
    *size = i + 1;
    /* A stdin letisztítása ha túl sok karakter volt az input */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return ERROR;
}


/**
 * @ingroup ui
 * @brief A karakterek kisbetűssé változtatása
 * @param c
 * @return char 
 */
char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')return c + 'a' - 'A';
    else return c;
}


/**
 * @ingroup ui
 * @brief Visszaadja a megfelelő kiírandó karakert egy adott bábutípushoz.
 * @param pc_t 
 * @return char 
 */
char getc_p(piece_t pc_t)
{
    switch (pc_t) 
    {
        case PAWN:   return 'P';
        case ROOK:   return 'R';
        case KNIGHT: return 'N';
        case BISHOP: return 'B';
        case QUEEN:  return 'Q';
        case KING:   return 'K';
        default:     return ' ';
    }
}


/**
 * @ingroup ui
 * @brief Kirajzolja az adott táblát a konzolra.
 * @param brd 
 */
void ui_drawboard(board brd)
{
    char *top =   ".---.---.---.---.---.---.---.---. \n";
    char *mid =   "|---+---+---+---+---+---+---+---| \n";
    char *bot =   "'---'---'---'---'---'---'---'---' \n";
    char *under = "  a   b   c   d   e   f   g   h\n";
    printf("%s",top);
    for(int i = 0; i < 8; i++)
    {
        if(i > 0)printf("%s",mid);
        for(int j = 0; j < 8; j++)
        {
            char c = getc_p(brd.pcs[j][i].type);
            if(brd.pcs[j][i].col == BLACK) c = to_lower(c);
            printf("| %c ", c);
        }
        printf("| %d\n", 8-i);
    }
    printf("%s", bot);
    printf("%s", under);
}


/**
 * @ingroup ui
 * @brief Egy adott karakter file-számmá alakítása.
 * @param c 
 * @param out Az átalakított szám.
 * @return ERROR vagy SUCCESS attól függően, hogy sikerült-e. 
 */
int get_file_c(char c, int *out)
{
    c = to_lower(c);
    if( c >= 'a' && c <= 'h')
    {
        *out = c - 'a';
        return SUCCESS;
    }
    return ERROR;
}


/**
 * @ingroup ui
 * @brief Egy adott karakter row-számmá alakítása.
 * @param c 
 * @param out Az átalakított szám.
 * @return ERROR vagy SUCCESS attól függően, hogy sikerült-e. 
 */
int get_row_c(char c, int *out)
{
    if(c >= '1' && c <= '8')
    {
        *out = c - '1';
        *out = 7 - *out;
        return SUCCESS;
    }
    return ERROR;
}


/**
 * @ingroup ui
 * @brief A promotion bábu typusának megállapítása
 * @param c
 * @return piece_t 
 */
piece_t get_piece_c(char c)
{
    c = to_lower(c);
    switch(c)
    {
        case 'q': return QUEEN;
        case 'b': return BISHOP;
        case 'n': return KNIGHT;
        case 'r': return ROOK;
        default:return EMPTY;
    }
}


/**
 * @ingroup ui
 * @brief Feldolgozza a ui_getusrin által beolvasott szöveget.
 * @details A függvény a karaktersorozatból egy lépésprototípust hoz létre.
 * Csak 5 vagy 6 hosszú karaktersorozatokat fogad el.
 * @param pmv Ide másolja a létrehozott adatokat.
 * @param buff A karaktersorozat.(elejére mutató pointer)
 * @param size A karaktersorozat hossza
 * @return ERROR vagy SUCCESS attól függőe, hogy sikeres volt-e.
 */
int ui_parse_str(move *pmv, char *buff, int size)
{
    /* Feldolgozza a beérkező szöveget */
    if(size == 5 || size == 6)
    {
        /* A sorok és oszlopok beállítása segédfüggvényekkel, ha nem sikerül valamelyik
        a függvény is ERROR-al lép ki*/
        int filefrm, rowfrm, fileto, rowto;
        if(get_file_c(buff[0], &filefrm) == ERROR)return ERROR;
        if(get_file_c(buff[2], &fileto) == ERROR)return ERROR;
        if(get_row_c(buff[1], &rowfrm) == ERROR)return ERROR;
        if(get_row_c(buff[3], &rowto) == ERROR)return ERROR;

        pmv->from = p_createpos(filefrm, rowfrm);
        pmv->to = p_createpos(fileto, rowto);
        pmv->promotion_piece = get_piece_c(buff[4]);
        return SUCCESS;
    }
    return ERROR;
}