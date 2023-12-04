#include <ncurses.h>
#include <string.h>

#include "jeu.c"
int main() {
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, TRUE);

    start_color();

    init_color(COLOR_RED, 255, 255, 0);
    init_pair(1, COLOR_YELLOW , COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3,COLOR_BLACK,COLOR_WHITE);
    init_pair(4, COLOR_WHITE,COLOR_BLACK);

    bkgdset(COLOR_PAIR(3));

    int ligne, colonne;
    getmaxyx(stdscr, ligne, colonne);

    // ASCII art
    const char *ascii_art[] = {
        " _____                                                                           _____ ",
        "( ___ )-------------------------------------------------------------------------( ___ )",
        " |   |                                                                           |   | ",
        " |   |                                                                           |   | ",
        " |   |     ______      _                              ______               _     |   | ",
        " |   |    (_____ \\    (_)                            (_____ \\             | |    |   | ",
        " |   |     _____) )___ _  ____ ____     ___  _   _    _____) )___  ____ _ | |    |   | ",
        " |   |    |  ____/ _  | |/ ___) _  )   / _ \\| | | |  |  ____/ _  )/ ___) || |    |   | ",
        " |   |    | |   ( ( | | | |  ( (/ /   | |_| | |_| |  | |   ( (/ /| |  ( (_| |    |   | ",
        " |   |    |_|    \\_||_|_|_|   \\____)   \\___/ \\____|  |_|    \\____)_|   \\____|    |   | ",
        " |   |                                                                           |   | ",
        " |___|                                                                           |___| ",
        "(_____)-------------------------------------------------------------------------(_____)"
    };

    const char *menu_items[] = {"Single", "Auto"};
    int choice;
    int highlight = 0;

    while (1) {
        clear();

        attron(COLOR_PAIR(3));
        for (int i = 0; i < 13; ++i) {
            mvprintw(ligne / 8 + i, (colonne - strlen(ascii_art[i])) / 2, ascii_art[i]);
        }
        attroff(COLOR_PAIR(highlight + 1));

        for (int i = 0; i < 2; ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(ligne / 2 + i, (colonne - strlen(menu_items[i])) / 2, menu_items[i]);
            attroff(A_REVERSE);
        }
        refresh();

        choice = getch();

        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + 2) % 2;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 2;
                break;
            case 10:
                if (highlight == 0) {
                    mvprintw(ligne / 2 + 2, (colonne - strlen("Single option chosen")) / 2, "Single option chosen");
                    refresh();
                    sleep(1);
                    clear();
                    refresh();
                    jeu(0);
                } else if (highlight == 1) {
                    mvprintw(ligne / 2 + 2, (colonne - strlen("Auto option chosen")) / 2, "Auto option chosen");
                    refresh();
                    sleep(1);
                    clear();
                    refresh();
                    jeu(1);
                }
                break;
        }
    }

    endwin();
    return 0;
}

