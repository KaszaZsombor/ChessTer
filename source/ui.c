#include <stdio.h>
#include "ui.h"

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
                *size = i;
                return ACCEPTED;
            }
            else return NOT_ACCEPTED;
        }
    }
    *size = i;
    /* A stdin letisztítása ha túl sok karakter volt az input */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return OVERFLOW;
}

char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')return c + 'a' - 'A';
    else return c;
}

char getc_p(piece_t piece)
{
    switch (piece) 
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
            char c = getc_p(brd.pcs[j][i]);
            if(brd.pcs_col[j][i] == BLACK) c = to_lower(c);
            printf("| %c ", c);
        }
        printf("| %d\n", 8-i);
    }
    printf("%s", bot);
    printf("%s", under);
}