#include "header.h"

void checkend(WINDOW* card_wins[3][4], int selected[3][4], int autom,
    int force_end, int quitbyq)
{
    int all_matched = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (!selected[i][j]) {
                all_matched = 0;
                break;
            }
        }
        if (!all_matched)
            break;
    }
    int height = ligne / 8;
    int width = colonne;
    int start_y = ligne - height; // En haut
    int start_x = 0; // a gauche
    WINDOW* scorewin = newwin(height, width, start_y, start_x);
    box(scorewin, 0, 0);
    if (quitbyq == 1) {
        // Lecture et affichage des scores
        FILE* file = fopen("score.txt", "r");
        char line[100];
        int line_count = 0;
        while (fgets(line, sizeof(line), file) != NULL && line_count < 3) {
            mvwprintw(scorewin, line_count + 2, 1, "%s", line);
            line_count++;
        }
        fclose(file);
        wrefresh(scorewin);
        mvwprintw(scorewin, start_x + 3,
            colonne / 2 - strlen("Appuyez pour quitter...") / 2,
            "Appuyez pour quitter...");

        wgetch(scorewin);
        delwin(scorewin);
        endwin();
        exit(0);
    };
    if (all_matched || force_end == 1) {

        float player_score = clock();
        player_score /= 1000000;

        // Lecture et affichage des scores
        FILE* file = fopen("score.txt", "r");
        char line[100];
        int line_count = 0;
        while (fgets(line, sizeof(line), file) != NULL && line_count < 3) {
            mvwprintw(scorewin, line_count + 2, 1, "%s", line);
            line_count++;
        }
        fclose(file);
        wrefresh(scorewin);

        if (autom == 1) {
            char botname[4] = "vBOT";
            update_score_file(player_score, botname);
            mvwprintw(scorewin, start_x + 3,
                colonne / 2 - strlen("Appuyez pour quitter...") / 2,
                "Appuyez pour quitter...");

            wgetch(scorewin);
        } else {
            mvwprintw(scorewin, start_x + 1, colonne / 2 - strlen("VICTOIRE"),
                "VICTOIRE");
            mvwprintw(scorewin, start_x + 5,
                colonne / 2
                    - (strlen("Enter your pseudonym (up to 4 characters): ")
                        / 2),
                "Enter your pseudonym (up to 4 characters): ");
            wrefresh(scorewin);

            // Saisie personnalisée du pseudonyme
            char pseudonym[5] = { 0 };
            int index = 0;
            int ch;
            while (index < 4) {
                ch = wgetch(scorewin);

                if (ch == '\n') {
                    break;
                } else if (ch != ERR && ch != '\t' && ch != '\b') {
                    pseudonym[index++] = ch;
                    wprintw(scorewin, "%c", ch);
                    wrefresh(scorewin);
                }
            };
            if (strlen(pseudonym) == 0) {
                strcpy(pseudonym, "user");
            }
            // Mise à jour du fichier de score
            update_score_file(player_score, pseudonym);
            mvwprintw(scorewin, start_x + 1,
                colonne / 2 - (strlen("Nouveaux scores !") / 2),
                "Nouveaux scores !");
            // Lecture et affichage des scores
            FILE* file = fopen("score.txt", "r");
            char line[100];
            int line_count = 0;
            while (fgets(line, sizeof(line), file) != NULL && line_count < 3) {
                mvwprintw(scorewin, line_count + 2, 1, "%s", line);
                line_count++;
            }
            fclose(file);
            wrefresh(scorewin);
            mvwprintw(scorewin, start_x + 3,
                colonne / 2 - strlen("Appuyez pour quitter...") / 2,
                "Appuyez pour quitter...");
            wgetch(scorewin);
        }

        delwin(scorewin); // Suppression de la fenêtre des scores
        endwin(); // Quitter le mode ncurses
        exit(0);
    }
}

void checkWindowSize()
{

    if (colonne < 170 || ligne < 60) {
        clear();
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "LA FENETRE DOIT FAIRE 170x60 AU MINIMUM.");
        attroff(COLOR_PAIR(1));
        refresh();
        getmaxyx(stdscr, ligne, colonne);
        exit(-1);
    }
    TextBox();
}


void movecard(int* y, int* x, int direction, int selected[3][4],
    int first_pick_y, int first_pick_x)
{

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

    while ((*y == first_pick_y && *x == first_pick_x) || selected[*y][*x]) {
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
    }
}
