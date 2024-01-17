#include "header.h"

void draw_card(WINDOW* card_win, int revealed, int value, int matched)
{
    werase(card_win);
    box(card_win, 0, 0);
    if (matched) {
        wbkgd(card_win, COLOR_PAIR(2));
        mvwprintw(card_win, CARD_HEIGHT / 2, (CARD_WIDTH - 1) / 2, "%d", value);
        mvwprintw(card_win, CARD_HEIGHT / CARD_HEIGHT, CARD_WIDTH / CARD_WIDTH,
            "%s", "lock");

    } else if (revealed) {
        wbkgd(card_win, COLOR_PAIR(4));
        mvwprintw(card_win, CARD_HEIGHT / 2, (CARD_WIDTH - 1) / 2, "%d", value);
    } else {
        wbkgd(card_win, COLOR_PAIR(0));
        mvwprintw(card_win, CARD_HEIGHT / 2, (CARD_WIDTH - 1) / 2, "%s", "?");
    }
    wnoutrefresh(card_win);
}

void TextBox()
{
    int ligne, colonne;
    getmaxyx(stdscr, ligne, colonne);
    int height = ligne / 9;
    int width = colonne;
    int start_y = 0; // En haut
    int start_x = 0; // a gauche

    WINDOW* welcome_win = newwin(height, width, start_y, start_x);
    box(welcome_win, 0, 0);

    mvwprintw(welcome_win, 2,
        (width
            - strlen("Bienvenue dans le jeu de memo ! Choisis rapidement, le "
                     "temps passe vite !"))
            / 2,
        "Bienvenue dans le jeu de memo ! Choisis rapidement, le temps passe "
        "vite "
        "!");

    wrefresh(welcome_win);
}
