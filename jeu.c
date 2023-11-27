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

    // Boucle pour le chronomčtre
    while (1) {
        // Récupération du temps actuel
        clock_gettime(CLOCK_MONOTONIC, &current_time);

        // Calcul du temps écoulé en secondes avec une précision de 0.1 seconde
        chronoactuel = (double)(current_time.tv_sec - start_time.tv_sec) +
                       (double)(current_time.tv_nsec - start_time.tv_nsec) / 1e9;

        // Affichage du temps dans la fenętre de droite
        mvwprintw(timerWin, 1, 1, "Chrono : %.1fs", chronoactuel);

        // Rafraîchissement de la fenętre de droite
        wrefresh(timerWin);

        // Pause de 100 millisecondes
        usleep(100000);
    }return NULL;
}

pthread_mutex_t ncurses_mutex;
void* updateText(void* arg) {
    WINDOW* mainWin = (WINDOW*)arg;
    char text[] = "Le but du jeu est simple : Trouve un couple de carte le plus rapidement possible ! Fais attention, le temps passe vite chef ! ";
    int textLength = strlen(text);
    int winWidth = getmaxx(mainWin);

    while (1) {
        for (int i = 0; i < textLength; i++) {
            int x, y;
            getyx(mainWin, y, x);
            pthread_mutex_lock(&ncurses_mutex);
    werase(mainWin);

            for (int j = 0; j < winWidth-2; j++) {
                int idx = (i + j) % textLength;
                mvwaddch(mainWin, 2, j + 1, text[(idx + textLength) % textLength]);
            }

            box(mainWin, 0, 0);
            wrefresh(mainWin);
    pthread_mutex_unlock(&ncurses_mutex);
            // utilisation de nanosleep --> plus robuste
            struct timespec req, rem;
            req.tv_sec = 0;
            req.tv_nsec = 300000 * 1000;  // 300000 microS vers nanoS
            nanosleep(&req, &rem);
        }
    }
}



int main() {
    pthread_mutex_init(&ncurses_mutex, NULL);
    initscr();
    noecho();
    curs_set(0);
    start_color();  // Activer la couleur
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // paire de couleur (jaune sur fond noir pour l'instant, on change peut etre après)

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int haut = 30;
    int large = 50;
    int mainWinWidth = (xMax / 2)-3;
    int mainWinHeight = (yMax/6)-2;
    int mainWinX = 1;
    int mainWinY = 1;
    WINDOW *mainWin = newwin(mainWinHeight, mainWinWidth, mainWinY, mainWinX);
    refresh();

    int timerWinWidth = (xMax / 2)-3 ;
    int timerWinHeight = (yMax/6)-2;
    int timerWinX = xMax - 79;
    int timerWinY = 1;
    WINDOW *timerWin = newwin(timerWinHeight, timerWinWidth, timerWinY, timerWinX);
    box(timerWin, 0, 0);
    wrefresh(timerWin);

    int textWinWidth = (xMax / 2)-3 ;
    int textWinHeight = (yMax/6)-2;
    int textWinX = 1;
    int textWinY = 1;
    WINDOW *textWin = newwin(textWinHeight, textWinWidth, textWinY, textWinX);
    box(textWin, 0, 0);
    wrefresh(textWin);


    pthread_t timerThread, textThread;
    pthread_create(&timerThread, NULL, updateTimer, timerWin);
    pthread_create(&textThread, NULL, updateText, textWin);

    /* affichage des cartes (dans un cadrillage 4*3)
    faut archi pas toucher a ça sinon ça part en couille les chefs*/

    int rows = 3;
    int cols = 4;

    // taille de la carte
    int cardWidth = 15;
    int cardHeight = 10;
    // espacement de la grille
    int horizontalSpacing = 0;
    int verticalSpacing = 1;

    int gridWidth = (cols * cardWidth);
    int gridHeight = (rows * cardHeight) + ((rows - 1) * verticalSpacing);

    int gridX = (xMax - gridWidth) / 2;
    int gridY = 10;

    // Créer la fenêtre de la grille
    WINDOW *gridWin = newwin(gridHeight, gridWidth, gridY, gridX);
    wrefresh(gridWin);

    int cardX = gridX;
    int cardY = gridY;
    int selectedX = cardX;
    int selectedY = cardY;
    int selectedCard = 0;  // index de la carte sélectionnée

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            WINDOW *cardWin = newwin(cardHeight, cardWidth, cardY, cardX);
            wattron(cardWin, COLOR_PAIR(1));  // Activer la couleur jaune
            box(cardWin, 0, 0);
            wattroff(cardWin, COLOR_PAIR(1));  // Désactiver la couleur jaune
            pthread_mutex_lock(&ncurses_mutex);
    wrefresh(cardWin);
    pthread_mutex_unlock(&ncurses_mutex);

            cardX += cardWidth + horizontalSpacing;
        }
        cardX = gridX;
        cardY += cardHeight + verticalSpacing;
    }

    while (1) {
        int ch = getch();
        // switch de carte
        WINDOW *previousCard = newwin(cardHeight, cardWidth, selectedY, selectedX);
        wattron(previousCard, COLOR_PAIR(1));  // Activer la couleur jaune
        box(previousCard, 0, 0);
        wattroff(previousCard, COLOR_PAIR(1));  // Désactiver la couleur jaune
        wrefresh(previousCard);

        // gestion des inputs
        if (ch == 'a' && selectedCard > 0) {
            selectedCard--;
        } else if (ch == 'e' && selectedCard < (rows * cols - 1)) {
            selectedCard++;

        }else if(ch == 'q') {
            break;
            }

        // calcul de posiitonnement de carte
        selectedX = gridX + (selectedCard % cols) * (cardWidth + horizontalSpacing);
        selectedY = gridY + (selectedCard / cols) * (cardHeight + verticalSpacing);

        attron(COLOR_PAIR(1));  // Activer la couleur jaune
        WINDOW *selectedCardWin = newwin(cardHeight, cardWidth, selectedY, selectedX);
        box(selectedCardWin, 0, 0);
        wrefresh(selectedCardWin);
        attroff(COLOR_PAIR(1));  // Désactiver la couleur jaune
    }

    delwin(mainWin);
    delwin(timerWin);
    delwin(textWin);
    delwin(gridWin);
    getch();
    endwin();
    pthread_cancel(timerThread);
    pthread_cancel(textThread);
    pthread_join(timerThread, NULL);
    pthread_join(textThread, NULL);

    return 0;
}
