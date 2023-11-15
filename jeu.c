#include <ncurses.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int minutes;
    int seconds;
    int milliseconds;
} Timer;

void* updateTimer(void* arg) {
    Timer* timer = (Timer*)arg;

    while (1) {
        usleep(10000);
        timer->milliseconds += 10;

        if (timer->milliseconds >= 1000) {
            timer->milliseconds -= 1000;
            timer->seconds++;

            if (timer->seconds >= 60) {
                timer->seconds -= 60;
                timer->minutes++;
            }
        }
    }

    return NULL;
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
            usleep(100000);  // Sleep for 100 milliseconds
        }
    }
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    start_color();  // Activer la couleur
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Définir une paire de couleurs (jaune sur fond noir pour l'instant, on change peut etre après)

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

    Timer timer = {0, 0, 0};
    pthread_t timerThread, textThread;
    pthread_create(&timerThread, NULL, updateTimer, &timer);
    pthread_create(&textThread, NULL, updateText, textWin);

    /* affichage des cartes (dans un cadrillage 4*3)
    faut archi pas toucher a ça sinon ça part en couille les chefs*/
    
    int rows = 3;
    int cols = 4;

    // taille de la carte
    int cardWidth = 15;
    int cardHeight = 10;
    // espeacement de la grille
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
    int selectedCard = 0;  // Indice de la carte sélectionnée

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            WINDOW *cardWin = newwin(cardHeight, cardWidth, cardY, cardX);
            wattron(cardWin, COLOR_PAIR(1));  // Activer la couleur jaune
            box(cardWin, 0, 0);
            wattroff(cardWin, COLOR_PAIR(1));  // Désactiver la couleur jaune
            wrefresh(cardWin);

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

    getch();
    endwin();
    pthread_cancel(timerThread);
    pthread_cancel(textThread);
    pthread_join(timerThread, NULL);
    pthread_join(textThread, NULL);

    return 0;
}
