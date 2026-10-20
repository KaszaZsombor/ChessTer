#include <stdio.h>
#include "ui.h"

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