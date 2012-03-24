
#include "toroidalui.h"

/* Menu pointers */
void (*tui_functors[10][10])(void);


/* Menu position */
int tui_pos_i = 0;
int tui_pos_j = 0;

void tui_register(int i, int j, void(*f)(void))
{
    tui_functors[i][j] = /*(void*)*/ f;
}

void tui_draw()
{
    tui_functors[tui_pos_i][tui_pos_j]();
}

void tui_up()
{
    tui_pos_i--;
    if(tui_pos_i < 0) tui_pos_i = 0;
}

void tui_down()
{
    tui_pos_i++;
}

void tui_left()
{
    tui_pos_j--;
    if(tui_pos_j < 0) tui_pos_j = 0;
}

void tui_right()
{
    tui_pos_j++;
}
