#include "header.h"

float chrono(clock_t debut, int colonne, int ligne)
{
    clock_t now = clock();
    // Calculate the elapsed time in seconds
    float sec = (float)(now - debut) / CLOCKS_PER_SEC;

    // Display the elapsed time on the screen
    attron(COLOR_PAIR(1));
    mvprintw(ligne / 5, (colonne / 2) - 8, "Chrono: %.1f", sec);
    attroff(COLOR_PAIR(2));
    refresh();

    return sec;
}

int waiting(int nbsec, float chrono_debut_lapsedeteemps)
{
    // Calculate the time elapsed since 'chrono_debut_lapsedeteemps'
    float laps_de_temps
        = chrono(debut, colonne, ligne) - chrono_debut_lapsedeteemps;

    // Check if the elapsed time is less than the specified 'nbsec'
    return nbsec > laps_de_temps;
}
