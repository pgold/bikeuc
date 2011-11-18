
#ifndef TOROIDALUI_H
#define TOROIDALUI_H

void tui_register(int i, int j, void(*f)(void));

void tui_draw();

void tui_up();
void tui_down();
void tui_left();
void tui_right();

#endif /* TOROIDALUI_H */