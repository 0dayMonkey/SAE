#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <time.h>


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
    wnoutrefresh(card_win);
}


float chrono(clock_t debut) {
    clock_t now = clock();
    float sec = (float)(now - debut) / 1000000;
    return sec;
}

void movecard(int *y, int *x, int direction, int selected[3][4], int first_pick_y, int first_pick_x) {
    int original_y = *y;
    int original_x = *x;

    *x += direction;

     if (*x > 3) {
        *x = 0;
        *y += 1;
    } else if (*x < 0) {
        *x = 3;
        *y -= 1;
    }

     if (*y > 2) {
        *y = 0;
    } else if (*y < 0) {
        *y = 2;
    }

     if ((*y == first_pick_y && *x == first_pick_x) || selected[*y][*x]) {
        *x += direction;
    }

     if (*x > 3) {
        *x = 0;
        *y += 1;
    } else if (*x < 0) {
        *x = 3;
        *y -= 1;
    }

     if (*y > 2) {
        *y = 0;
    } else if (*y < 0) {
        *y = 2;
    }

}









int fin = 0;
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
        fin = 1;
        usleep(2000000);
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

void TextBox() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int height = max_y/10;
    int width = max_x;
    int start_y = 0; // En haut
    int start_x = 0; // a gauche

    WINDOW *welcome_win = newwin(height, width, start_y, start_x);
    box(welcome_win, 0, 0);

    mvwprintw(welcome_win, 3, (width - strlen("Bienvenue dans le jeu de memo ! Choisis rapidement, le temps passe vite !")) / 2, "Bienvenue dans le jeu de memo ! Choisis rapidement, le temps passe vite !");

    wrefresh(welcome_win);
}


void checkWindowSize() {
    int width, height;
    getmaxyx(stdscr, height, width);

    while (width < 170 || height < 60) {
        clear();
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "LA FENETRE DOIT FAIRE 170x60 AU MINIMUM.");
        attroff(COLOR_PAIR(1));
        refresh();
        getmaxyx(stdscr, height, width);
    }
    TextBox();
}



// nouvelle selection random, ca retourne un booleen pour savoir si on appuie sur entrer
bool SelectionRandom(clock_t debut, int *current_y, int *current_x, int selected[3][4], int first_pick_y, int first_pick_x) {
    int random_move = rand() % 12 + 1;
    for (int i = 0; i < random_move; i++) {

        refresh();
        usleep(100000);
        movecard(current_y, current_x, 1, selected, first_pick_y, first_pick_x);
    }
    return true;
}







int jeu(int autom) {
    WINDOW *card_wins[3][4];
    int card_values[3][4];
    int revealed[3][4] = {{0}};
    int selected[3][4] = {{0}};
    int start_y, start_x;
    int ch;
    int current_y = 0, current_x = 0;
    int first_pick_y = -1, first_pick_x = -1;
    int second_pick_y = -1, second_pick_x = -1;
    char strchrono[20]="Chrono:";
    srand(time(0));
    clock_t debut = clock();


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
    TextBox();



    // valeurs des cartes
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


    // point de départ pour centrer les cartes
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
                        doupdate();
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



    // Boucle principale
    nodelay(stdscr, TRUE);

    while ((ch = getch()) != 'q') {


        checkWindowSize();

        attron(COLOR_PAIR(1));
        mvprintw(max_y/5, (max_x/2)-strlen(strchrono),"Chrono: %.1f", chrono(debut));
        attroff(COLOR_PAIR(2));
        refresh();

        if(autom == 1){

            if (SelectionRandom(debut, &current_y, &current_x, selected, first_pick_y, first_pick_x)) {

                ch = '\n';
            }
}

        switch (ch) {
            case 'a':
                movecard(&current_y, &current_x, -1,selected,first_pick_y, first_pick_x);

                break;
            case 'e':

                movecard(&current_y, &current_x, 1, selected,first_pick_y, first_pick_x);
                break;
            case '\n':
                if (revealed[current_y][current_x] == 0) {
                    // si aucune carte n'est actuellement sélectionnée
                    if (first_pick_y == -1) {
                        first_pick_y = current_y;
                        first_pick_x = current_x;
                        revealed[current_y][current_x] = 1;
                        movecard(&current_y, &current_x, 1, selected,first_pick_y, first_pick_x);
                    } else if (!(current_y == first_pick_y && current_x == first_pick_x)) {
                        second_pick_y = current_y;
                        second_pick_x = current_x;
                        revealed[current_y][current_x] = 1;
                        draw_card(card_wins[current_y][current_x], revealed[current_y][current_x], card_values[current_y][current_x],debug);
                        doupdate();
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
                                usleep(2000000);
                                movecard(&current_y, &current_x, 1, selected,first_pick_y, first_pick_x);

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

