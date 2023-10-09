#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tools.h"
#include "jeu.c"

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    // Check window size
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    if (max_x < 170 || max_y < 60) {
        endwin();
        fprintf(stderr, "La taille de la fenetre n'est pas assez consequente. Modifiez les dimensions minimum à 170x60.\n");
        return 1;
    }

    // colors
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(2, COLOR_CYAN, COLOR_BLACK);
    }

    getmaxyx(stdscr, max_y, max_x); // dim
    int old_max_x = max_x; // old width
    int old_max_y = max_y; // old height

    char* ascii_art[] = {
        "   .----------------.  .----------------.  .----------------.  .----------------.  .----------------. ",
        "  | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |",
        "  | |     __       | || |   ______     | || |      __      | || |     _____    | || |  _______     | |",
        "  | |    /  |      | || |  |_   __ \\   | || |     /  \\     | || |    |_   _|   | || | |_   __ \\    | |",
        "  | |    `| |      | || |    | |__) |  | || |    / /\\ \\    | || |      | |     | || |   | |__) |   | |",
        "  | |     | |      | || |    |  ___/   | || |   / ____ \\   | || |      | |     | || |   |  __ /    | |",
        "  | |    _| |_     | || |   _| |_      | || | _/ /    \\ \\_ | || |     _| |_    | || |  _| |  \\ \\_  | |",
        "  | |   |_____|    | || |  |_____|     | || ||____|  |____|| || |    |_____|   | || | |____| |___| | |",
        "  | |              | || |              | || |              | || |              | || |              | |",
        "  | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |",
        "   '----------------'  '----------------'  '----------------'  '----------------'  '----------------' "
    };

    int art_height = 11;
    int art_width = strlen(ascii_art[0]);

    int start_x = (max_x - art_width) / 2;
    int start_y = 10;

    int current_color = 1; // Couleur actuelle
    int previous_option = -1;

    // options
    char* menu_options[] = {
        "1 - Solo",
        "2 - Autoplay"
    };
    int menu_option_colors[] = {1, 1}; // synch color

    int selected_option = 0;

    while (1) {
        clear();

        // border box
        box(stdscr, 0, 0);

        // adaptive
        getmaxyx(stdscr, max_y, max_x);
        if (max_x != old_max_x || max_y != old_max_y) {

            old_max_x = max_x;
            old_max_y = max_y;
            start_x = (max_x - art_width) / 2;
        }

        for (int i = 0; i < 2; i++) {
            if (i == selected_option) {
                attron(COLOR_PAIR(current_color));
                attron(A_BOLD);
            }
            mvprintw(max_y / 2 - 2 + i, max_x / 2 - strlen(menu_options[i]) / 2, "%s", menu_options[i]);
            if (i == selected_option) {
                attroff(A_BOLD);
                attroff(COLOR_PAIR(current_color));
            }
        }

        if (selected_option != previous_option) {
            // La sélection a changé, changez la couleur
            current_color = (current_color % 2) + 1;
            previous_option = selected_option;
        }

        attron(COLOR_PAIR((current_color%2)+1));
        for (int i = 0; i < art_height; i++) {
            mvprintw(start_y + i, start_x, "%s", ascii_art[i]);
        }
        attroff(COLOR_PAIR((current_color%2)+1));

        refresh();

        int ch = getch();

        if (ch == KEY_DOWN || ch == '2' || ch == 's') {
            selected_option = (selected_option + 1) % 2; // Passe à l'option suivante
        } else if (ch == KEY_UP || ch == '1' || ch == 'z') {
            selected_option = (selected_option - 1 + 2) % 2; // Passe à l'option précédente
        } else if (ch == 10) {
            initialiserJeu();
        }
        else if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                // MOUSE_CLICK event
                if (event.y >= max_y / 2 - 2 && event.y < max_y / 2) {
                    for (int i = 0; i < 2; i++) {
                        if (event.y == max_y / 2 - 2 + i) {
                            selected_option = i;
                            break;
                        }
                    }
                }
            }
        }  else if (ch == 'q') {
            break; // 'q' exit
        }
    }

    endwin(); // End ncurses mode
    return 0;
}
