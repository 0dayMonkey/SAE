#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CARD_WIDTH 15
#define CARD_HEIGHT 10
#define PADDING 1
#define PAIR_COUNT 6

/// mod
int debug = 0; // 1 = triche et 0 = normal



void draw_card(WINDOW *card_win, int revealed, int value, int matched) {
    werase(card_win);
    box(card_win, 0 , 0);
    if (matched) {
        wbkgd(card_win, COLOR_PAIR(2));
        mvwprintw(card_win, CARD_HEIGHT / 2, (CARD_WIDTH - 1) / 2, "%d", value);
        mvwprintw(card_win, CARD_HEIGHT/CARD_HEIGHT, CARD_WIDTH/CARD_WIDTH , "%s","lock");

    }
    else if (revealed) {
        wbkgd(card_win, COLOR_PAIR(4));
        mvwprintw(card_win, CARD_HEIGHT / 2, (CARD_WIDTH - 1) / 2, "%d", value);
    } else {
        wbkgd(card_win, COLOR_PAIR(0));
        mvwprintw(card_win, CARD_HEIGHT / 2, (CARD_WIDTH - 1) / 2, "%s", "?");
    }
    wrefresh(card_win);
}


void movecard(int *y, int *x, int selected[3][4], int direction) {
    do {
        *x += direction;
        if (*x > 3) { // Passer à la ligne suivante
            *x = 0; *y += 1;
        } else if (*x < 0) { // Remonter à la ligne précédente
            *x = 3; *y -= 1;
        }
        if (*y > 2) { // Si on dépasse la dernière ligne, retourner au début
            *y = 0;
        } else if (*y < 0) { // Si on dépasse la première ligne, aller à la fin
            *y = 2;
        }
    } while (*y >= 0 && *y < 3 && *x >= 0 && *x < 4 && selected[*y][*x]); // Continuer tant que la carte est sélectionnée
}


void checkend(WINDOW *card_wins[3][4], int selected[3][4]) {
    int all_matched = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (!selected[i][j]) {
                all_matched = 0;
                break;
            }
        }
        if (!all_matched) break;
    }

    if (all_matched) {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        clear();
        mvprintw((max_y / 2), (max_x - 2) / 2, "Gestion des scores ici"); /// remplacer par les scores
        refresh();
        usleep(3000000);
        endwin();
        exit(0);
    }
}


int jeu() {
    WINDOW *card_wins[3][4];
    int card_values[3][4];
    int revealed[3][4] = {{0}};
    int selected[3][4] = {{0}};
    int start_y, start_x;
    int ch;
    int current_y = 0, current_x = 0;
    int first_pick_y = -1, first_pick_x = -1;
    int second_pick_y = -1, second_pick_x = -1;

    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    srand(time(NULL));


    // Initialisation des valeurs des cartes
    int values[PAIR_COUNT * 2];
    for (int i = 0; i < PAIR_COUNT; ++i) {
        values[i * 2] = values[i * 2 + 1] = i + 1;
    }
    for (int i = 0; i < PAIR_COUNT * 2; ++i) {
        int r = rand() % (PAIR_COUNT * 2);
        int temp = values[i];
        values[i] = values[r];
        values[r] = temp;
    }

    int max_y, max_x; // stocker les dimensions de l'écran
    getmaxyx(stdscr, max_y, max_x); // dimensions de l'écran


    // Calculer le point de départ pour centrer les cartes
    int total_cards_width = (CARD_WIDTH + PADDING) * 4 - PADDING;
    int total_cards_height = (CARD_HEIGHT + PADDING) * 3 - PADDING;
    start_y = (max_y - total_cards_height) / 2;
    start_x = (max_x - total_cards_width) / 2;

    // positionner les cartes
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            card_wins[i][j] = newwin(CARD_HEIGHT, CARD_WIDTH, start_y + i * (CARD_HEIGHT + PADDING), start_x + j * (CARD_WIDTH + PADDING));
            if (i == 0 && j == 0) {
                        // highlight la premiere carte ( au demarage du jeu )
                        wattron(card_wins[i][j], COLOR_PAIR(1));
                        draw_card(card_wins[i][j], revealed[i][j], ' ', debug);
                        wattroff(card_wins[i][j], COLOR_PAIR(1));
                    } else {
                        draw_card(card_wins[i][j], revealed[i][j], ' ', debug);
                    }
                }
            }

    // donner les valeurs mélangées aux cartes
    for (int i = 0, k = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++, k++) {
            card_values[i][j] = values[k];
            card_wins[i][j] = newwin(CARD_HEIGHT, CARD_WIDTH, start_y + (CARD_HEIGHT + PADDING) * i, start_x + (CARD_WIDTH + PADDING) * j);
        }
    }


    /// A REGLER : probleme lorsqu'on veux passer de la premiere a la derniere carte ( on ne saute plus de carte )
    // Boucle principale
    while ((ch = getch()) != 'q') {

        switch (ch) {
            case 'a':
                if (current_x==first_pick_x+1 && current_y==first_pick_y){
                    movecard(&current_y, &current_x, selected, -1);
                }
                movecard(&current_y, &current_x, selected, -1);
                break;
            case 'e':
                if (current_x==first_pick_x-1 && current_y==first_pick_y){
                    movecard(&current_y, &current_x, selected, 1);
                }
                movecard(&current_y, &current_x, selected, 1);
                break;
            case '\n':
                if (revealed[current_y][current_x] == 0) {
                    // si aucune carte n'est actuellement sélectionnée
                    if (first_pick_y == -1) {
                        first_pick_y = current_y;
                        first_pick_x = current_x;
                        revealed[current_y][current_x] = 1;
                        movecard(&current_y, &current_x, selected, 1);
                    } else if (!(current_y == first_pick_y && current_x == first_pick_x)) {
                        second_pick_y = current_y;
                        second_pick_x = current_x;
                        revealed[current_y][current_x] = 1;
                        draw_card(card_wins[current_y][current_x], revealed[current_y][current_x], card_values[current_y][current_x],debug);
                        //  comparaison
                        if (card_values[first_pick_y][first_pick_x] == card_values[second_pick_y][second_pick_x]) {
                                // cartes sont egales
                                selected[first_pick_y][first_pick_x] = 1;
                                selected[second_pick_y][second_pick_x] = 1;
                                revealed[first_pick_y][first_pick_x] = 1;
                                revealed[second_pick_y][second_pick_x] = 1;
                                // peut-être ajouter un refresh ici
                                wrefresh(card_wins[first_pick_y][first_pick_x]);
                                wrefresh(card_wins[second_pick_y][second_pick_x]);
                                checkend(card_wins, selected);
                                usleep(1000000);
                                movecard(&current_y, &current_x, selected, 1);

                        } else {
                            // pas de correspondance on retourne les cartes apre 1s
                            usleep(1000000);
                            revealed[first_pick_y][first_pick_x] = 0;
                            revealed[second_pick_y][second_pick_x] = 0;




                        }
                        first_pick_y = -1;
                        first_pick_x = -1;
                        second_pick_y = -1;
                        second_pick_x = -1;
                    }
                }
                break;
        }

        // refresh les cartes
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                int is_matched = selected[i][j];
                draw_card(card_wins[i][j], revealed[i][j], card_values[i][j], is_matched);
            }
        }



        // highlight
        if (!selected[current_y][current_x]) {
            wattron(card_wins[current_y][current_x], COLOR_PAIR(1));
            draw_card(card_wins[current_y][current_x], revealed[current_y][current_x], card_values[current_y][current_x],debug);
            wattroff(card_wins[current_y][current_x], COLOR_PAIR(1));
        }
    }

    endwin();

    return 0;
}
