// SEULEMENT LORSQUE Q EST PRESSE OU TOUTES LES CARTES SONT RETOURNEES ET LE CHRONO S'ARRETE!

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#define TABLEAU_SCORES_TAILLE 3
#define FICHIER_SCORES "jeuhighscore.txt"


typedef struct {
    int classement;
    char nom[5 + 1]; // Augmentation de la taille pour inclure le caractère nul
    double chrono;
} Score;

void trierMeilleursScores(Score scores[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (scores[j].chrono > scores[j + 1].chrono) {
                Score temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

void ajusterClassements(Score scores[], int n) {
    for (int i = 0; i < n; i++) {
        scores[i].classement = i + 1;
    }
}

int main() {
    initscr();
    noecho();
    cbreak();

    Score meilleursScores[TABLEAU_SCORES_TAILLE];

    FILE *fichier = fopen(FICHIER_SCORES, "r");

    if (!fichier) {
        printw("Le fichier de scores n'existe pas.\n");
        refresh();
        getch();
        endwin();
        return 1;
    }

    for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
        if (fscanf(fichier, "%d %4s %lf\n", &meilleursScores[i].classement, meilleursScores[i].nom, &meilleursScores[i].chrono) != 3) {
            printw("Erreur de lecture du fichier de scores.\n");
            refresh();
            getch();
            fclose(fichier);
            endwin();
            return 1;
        }
    }
    fclose(fichier);

    double chronoPartie = TEMPS_JOUEUR;

    if (chronoPartie < meilleursScores[TABLEAU_SCORES_TAILLE - 1].chrono) {
        char nomJoueur[5 + 1];
        printw("Bravo, vous avez battu un meilleur chrono ! Entrez votre nom (4 caractères SEULEMENT!) : ");
        refresh();
        getnstr(nomJoueur, 4);

        // Ajouter le nouveau score à la fin du tableau
        meilleursScores[TABLEAU_SCORES_TAILLE].classement = 0;
        strcpy(meilleursScores[TABLEAU_SCORES_TAILLE].nom, nomJoueur);
        meilleursScores[TABLEAU_SCORES_TAILLE].chrono = chronoPartie;

        // Trier le tableau
        trierMeilleursScores(meilleursScores, TABLEAU_SCORES_TAILLE + 1);

        // Ajuster les classements
        ajusterClassements(meilleursScores, TABLEAU_SCORES_TAILLE + 1);

        // Écrire les trois meilleurs scores dans le fichier
        fichier = fopen(FICHIER_SCORES, "w");
        if (fichier) {
            for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
                fprintf(fichier, "%d %4s %.1lf\n", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
            }
            fclose(fichier);
        }
    }

    printw("Meilleurs Scores :\n");
    for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
        printw("%d. %4s %.1lf\n", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
    }
    refresh();

    getch();
    endwin();
    return 0;
}
