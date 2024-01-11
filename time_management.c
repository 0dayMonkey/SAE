#include "header.h"

float chrono(clock_t debut, int colonne, int ligne)
{
  clock_t now = clock();
  float sec = (float)(now - debut) / 1000000;
  attron(COLOR_PAIR(1));
  mvprintw(ligne / 5, (colonne / 2) - 8, "Chrono: %.1f", sec);
  attroff(COLOR_PAIR(2));
  refresh();
  return sec;
}

int waiting(int nbsec, float chrono_debut_lapsedeteemps)
{
  float laps_de_temps
      = chrono(debut, colonne, ligne) - chrono_debut_lapsedeteemps;
  return nbsec > laps_de_temps;
}
