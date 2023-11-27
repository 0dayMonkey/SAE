#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

void* updateTimer(void* arg) {
    WINDOW* timerWin = (WINDOW*)arg;
    struct timespec start_time, current_time;
    double chronoactuel = 0;

    // Récupération du temps de départ
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Boucle pour le chronomètre
    while (1) {
        // Récupération du temps actuel
        clock_gettime(CLOCK_MONOTONIC, &current_time);

        // Calcul du temps écoulé en secondes avec une précision de 0.1 seconde
        chronoactuel = (double)(current_time.tv_sec - start_time.tv_sec) +
                       (double)(current_time.tv_nsec - start_time.tv_nsec) / 1e9;

        // Affichage du temps dans la fenêtre de droite
        mvwprintw(timerWin, 1, 1, "Chrono : %.1fs", chronoactuel);

        // Rafraîchissement de la fenêtre de droite
        wrefresh(timerWin);

        // Pause de 100 millisecondes
        usleep(100000);
    }
}

void* updateText(void* arg) {
    WINDOW* mainWin = (WINDOW*)arg;
    char text[] = "Le but du jeu est simple : Trouve un couple de carte le plus rapidement possible ! Fais attention, le temps passe vite chef ! ";
    int textLength = strlen(text);
    int winWidth = getmaxx(mainWin);

    while (1) {
        for (int i = 0; i < textLength; i++) {
            int x, y;
            getyx(mainWin, y, x);
            werase(mainWin);

            for (int j = 0; j < winWidth; j++) {
                int idx = (i + j) % textLength;
                mvwaddch(mainWin, 2, j + 1, text[(idx + textLength) % textLength]);
            }

            box(mainWin, 0, 0);
            wrefresh(mainWin);

            // Pause de 100 millisecondes
            usleep(100000);
        }
    }
}

int main() {
    // Initialisation de ncurses
    initscr();
    noecho();
    curs_set(0);
    start_color();  // Activer la couleur
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Définir une paire de couleurs (jaune sur fond noir pour l'instant, on change peut etre après)

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int haut = 30;
    int large = 50;
    int mainWinWidth = (xMax / 2) - 3;
    int mainWinHeight = (yMax / 6) - 2;
    int mainWinX = 1;
    int mainWinY = 1;
    WINDOW *mainWin = newwin(mainWinHeight, mainWinWidth, mainWinY, mainWinX);
    refresh();

    int timerWinWidth = (xMax / 2) - 3;
    int timerWinHeight = (yMax / 6) - 2;
    int timerWinX = xMax - 79;
    int timerWinY = 1;
    WINDOW *timerWin = newwin(timerWinHeight, timerWinWidth, timerWinY, timerWinX);
    box(timerWin, 0, 0);
    wrefresh(timerWin);

    int textWinWidth = (xMax / 2) - 3;
    int textWinHeight = (yMax / 6) - 2;
    int textWinX = 1;
    int textWinY = 1;
    WINDOW *textWin = newwin(textWinHeight, textWinWidth, textWinY, textWinX);
    box(textWin, 0, 0);
    wrefresh(textWin);

    pthread_t timerThread, textThread;
    pthread_create(&timerThread, NULL, updateTimer, timerWin);
    pthread_create(&textThread, NULL, updateText, textWin);

    int rows = 3;
    int cols = 4;

    int cardWidth = 15;
    int cardHeight = 10;
    int horizontalSpacing = 0;
    int verticalSpacing = 1;

    int gridWidth = (cols * cardWidth);
    int gridHeight = (rows * cardHeight) + ((rows - 1) * verticalSpacing);

    int gridX = (xMax - gridWidth) / 2;
    int gridY = 10;

    WINDOW *gridWin = newwin(gridHeight, gridWidth, gridY, gridX);
    wrefresh(gridWin);

    int cardX = gridX;
    int cardY = gridY;
    int selectedX = cardX;
    int selectedY = cardY;
    int selectedCard = 0;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            WINDOW *cardWin = newwin(cardHeight, cardWidth, cardY, cardX);
            wattron(cardWin, COLOR_PAIR(1));
            box(cardWin, 0, 0);
            wattroff(cardWin, COLOR_PAIR(1));
            wrefresh(cardWin);

            cardX += cardWidth + horizontalSpacing;
        }
        cardX = gridX;
        cardY += cardHeight + verticalSpacing;
    }

    while (1) {
        int ch = getch();

        WINDOW *previousCard = newwin(cardHeight, cardWidth, selectedY, selectedX);
        wattron(previousCard, COLOR_PAIR(1));
        box(previousCard, 0, 0);
        wattroff(previousCard, COLOR_PAIR(1));
        wrefresh(previousCard);

        if (ch == 'a' && selectedCard > 0) {
            selectedCard--;
        } else if (ch == 'e' && selectedCard < (rows * cols - 1)) {
            selectedCard++;
        }

        selectedX = gridX + (selectedCard % cols) * (cardWidth + horizontalSpacing);
        selectedY = gridY + (selectedCard / cols) * (cardHeight + verticalSpacing);

        attron(COLOR_PAIR(1));
        WINDOW *selectedCardWin = newwin(cardHeight, cardWidth, selectedY, selectedX);
        box(selectedCardWin, 0, 0);
        wrefresh(selectedCardWin);
        attroff(COLOR_PAIR(1));
    }

    getch();
    endwin();
    pthread_cancel(timerThread);
    pthread_cancel(textThread);
    pthread_join(timerThread, NULL);
    pthread_join(textThread, NULL);

    return 0;
}
