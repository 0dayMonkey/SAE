#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tools.h"



void initialiserJeu() {


    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    clear();

    refresh();
}

void jouer() {


    refresh();
}

void nettoyerJeu() {


    endwin();
}
