#include "header.h"

/// mod
int debug = 0; // 1 = triche et 0 = normal
int force_end = 0; // 0 non et 1 oui

/// GLOBAL
int ligne, colonne;
clock_t debut;
int quitbyq = 0; // 0 = non et 1 = oui






// nouvelle selection random, ca retourne un booleen pour savoir si on appuie
// sur entrer
bool SelectionRandom(clock_t debut, int* current_y, int* current_x,
    int selected[3][4], int first_pick_y, int first_pick_x)
{
  int random_move = rand() % 12 + 1;
  for (int i = 0; i < random_move; i++) {
    movecard(current_y, current_x, 1, selected, first_pick_y, first_pick_x);
  }
  return true;
}

int jeu(int autom)
{
  WINDOW* card_wins[3][4];
  int card_values[3][4];
  int revealed[3][4] = { { 0 } };
  int selected[3][4] = { { 0 } };
  int start_y, start_x;
  int current_y = 0, current_x = 0;
  int first_pick_y = -1, first_pick_x = -1;
  int second_pick_y = -1, second_pick_x = -1;
  srand(time(0));
  debut = clock();

  initialize_score_file();

  initscr();
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
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

  int ligne, colonne; // stocker les dimensions de l'écran
  getmaxyx(stdscr, ligne, colonne); // dimensions de l'écran

  // point de départ pour centrer les cartes
  int total_cards_width = (CARD_WIDTH + PADDING) * 4 - PADDING;
  int total_cards_height = (CARD_HEIGHT + PADDING) * 3 - PADDING;
  start_y = (ligne - total_cards_height) / 2;
  start_x = (colonne - total_cards_width) / 2;

  // positionner les cartes
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      card_wins[i][j] = newwin(CARD_HEIGHT, CARD_WIDTH,
          start_y + i * (CARD_HEIGHT + PADDING),
          start_x + j * (CARD_WIDTH + PADDING));
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
      card_wins[i][j] = newwin(CARD_HEIGHT, CARD_WIDTH,
          start_y + (CARD_HEIGHT + PADDING) * i,
          start_x + (CARD_WIDTH + PADDING) * j);
    }
  }

  // Boucle principale
  nodelay(stdscr, TRUE);
  checkWindowSize();
  bool deroulement = true;
  while (deroulement) {
    char ch = getch();
    chrono(debut, colonne, ligne);
    if (autom == 1) {
      SelectionRandom(
          debut, &current_y, &current_x, selected, first_pick_y, first_pick_x);
      ch = '\n';
    }

    // refresh les cartes
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        int is_matched = selected[i][j];

        draw_card(
            card_wins[i][j], revealed[i][j], card_values[i][j], is_matched);
      }
    }
    // highlight
    if (!selected[current_y][current_x]) {
      wattron(card_wins[current_y][current_x], COLOR_PAIR(1));
      draw_card(card_wins[current_y][current_x], revealed[current_y][current_x],
          card_values[current_y][current_x], debug);
      wattroff(card_wins[current_y][current_x], COLOR_PAIR(1));
    }

    switch (ch) {
    case 'q':
      checkend(card_wins, selected, autom, 0, 1);

    case 'w':
      checkend(card_wins, selected, autom, 1, 0);
    case 'a':
      movecard(
          &current_y, &current_x, -1, selected, first_pick_y, first_pick_x);

      break;
    case 'e':

      movecard(&current_y, &current_x, 1, selected, first_pick_y, first_pick_x);
      break;
    case '\n':

      if (revealed[current_y][current_x] == 0) {
        // si aucune carte n'est actuellement sélectionnée
        if (first_pick_y == -1) {
          first_pick_y = current_y;
          first_pick_x = current_x;
          revealed[current_y][current_x] = 1;
          if(!autom){movecard(&current_y, &current_x, 1, selected,first_pick_y,first_pick_x);};
        } else if (!(current_y == first_pick_y && current_x == first_pick_x)) {
          second_pick_y = current_y;
          second_pick_x = current_x;
          revealed[current_y][current_x] = 1;
          draw_card(card_wins[current_y][current_x],
              revealed[current_y][current_x], card_values[current_y][current_x],
              debug);
          doupdate();
          //  comparaison
          if (card_values[first_pick_y][first_pick_x]
              == card_values[second_pick_y][second_pick_x]) {
            // cartes sont egales
            selected[first_pick_y][first_pick_x] = 1;
            selected[second_pick_y][second_pick_x] = 1;

            // peut-être ajouter un refresh ici
            wrefresh(card_wins[first_pick_y][first_pick_x]);
            wrefresh(card_wins[second_pick_y][second_pick_x]);
            checkend(card_wins, selected, autom, 0, 0);
            float chrono_debut_lapsedeteemps = chrono(debut, colonne, ligne);
            while (waiting(2, chrono_debut_lapsedeteemps)) {
              chrono(debut, colonne, ligne);
            }
            flushinp();
            movecard(&current_y, &current_x, 1, selected, first_pick_y,
                first_pick_x);

          } else {
            // pas de correspondance on retourne les cartes apre 1s
            float chrono_debut_lapsedeteemps = chrono(debut, colonne, ligne);
            while (waiting(2, chrono_debut_lapsedeteemps)) {
              chrono(debut, colonne, ligne);
            }
            flushinp();
            revealed[first_pick_y][first_pick_x] = 0;
            revealed[second_pick_y][second_pick_x] = 0;
            wrefresh(card_wins[first_pick_y][first_pick_x]);
            wrefresh(card_wins[second_pick_y][second_pick_x]);
          }

          first_pick_y = -1;
          first_pick_x = -1;
          second_pick_y = -1;
          second_pick_x = -1;
        }
      }
      break;
    }
  }

  endwin();

  return 0;
}
