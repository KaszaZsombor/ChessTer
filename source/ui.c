/**
 * @file ui.c
 * @author Kasza Zsombor
 * @brief A felhasználó - program közti információküzlést végzi.
 * @version 0.1
 * @date 2026-10-21
 * 
 * @copyright Copyright (c) 2026
 * 
 */

 
#include <stdio.h>
#include "ui.h"
#include "move.h"


/* A kimenet a beolvasás sikerét határozza meg,
a size befele a maximum msg size, kifele a beolvasott size*/
int ui_getusrin(int *size, char *msg)
{
    int i;
    for(i = 0; i < *size; i++)
    {
        if(scanf("%c", &msg[i]) == EOF)return END;
        if(msg[i] == '\n')
        {
            if(i >= 4)
            {
                /* Ha megfelelő hosszú szöveget kaptunk*/
                *size = i + 1;
                return SUCCESS;
            }
            else
            {
                return ERROR;
            }
        }
    }
    *size = i + 1;
    /* A stdin letisztítása ha túl sok karakter volt az input */
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return ERROR;
}

char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')return c + 'a' - 'A';
    else return c;
}

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

/* Feldolgozza a beérkező szöveget */
/* Ide érkező szövegek hossza 5 vagy 6*/
int ui_parse_str(move *pmv, char *buff, int size)
{
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