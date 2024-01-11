extern int ligne, colonne;


#include <time.h>
extern clock_t debut;


#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CARD_WIDTH 15
#define CARD_HEIGHT 10
#define PADDING 1
#define PAIR_COUNT 6
///////////////////////// GESTION DES SCORES /////////////////////////

void initialize_score_file();
void update_score_file(float current_score, char *pseudonym);
///////////////////////////////////////////////////////////////////////////

void draw_card(WINDOW *card_win, int revealed, int value, int matched);
float chrono(clock_t debut, int colonne, int ligne);
int waiting(int nbsec, float chrono_debut_lapsedeteemps);
void movecard(int *y, int *x, int direction, int selected[3][4], int first_pick_y, int first_pick_x) ;
void checkend(WINDOW *card_wins[3][4], int selected[3][4], int autom, int force_end, int quitbyq);
void TextBox();

void checkWindowSize();

// nouvelle selection random, ca retourne un booleen pour savoir si on appuie sur entrer
bool SelectionRandom(clock_t debut, int *current_y, int *current_x, int selected[3][4], int first_pick_y, int first_pick_x);
int jeu(int autom);


#endif // HEADER_H_INCLUDED
