#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#define TABLEAU_SCORES_TAILLE 3
#define FICHIER_SCORES "jeuhighscore.txt"

typedef struct {
    int classement;
    char nom[5];
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

int main() {
    initscr();
    noecho();
    cbreak();

    Score meilleursScores[TABLEAU_SCORES_TAILLE];

    FILE* fichier = fopen(FICHIER_SCORES, "r");
    if (fichier) {
        for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
            if (fscanf(fichier, "%d %4s %lf\n", &meilleursScores[i].classement,
                meilleursScores[i].nom, &meilleursScores[i].chrono) != 3) {
                printw("Erreur de lecture du fichier de scores.\n");
                refresh();
                getch();
                endwin();
                return 1;
            }
        }
        fclose(fichier);
    } else {
        printw("Le fichier de scores n'existe pas. Création du fichier...\n");
        refresh();
        // Créez le fichier de scores avec des scores par défaut
        fichier = fopen(FICHIER_SCORES, "w");
        for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
            meilleursScores[i].classement = i + 1;
            strcpy(meilleursScores[i].nom, "AAA");
            meilleursScores[i].chrono = 999.9;  // Valeur élevée pour l'initialisation
            fprintf(fichier, "%d %4s %.1lf\n", meilleursScores[i].classement,
                meilleursScores[i].nom, meilleursScores[i].chrono);
        }
        fclose(fichier);
    }

    // Simulation d'une partie victorieuse avec un chrono de 80.0
    double chronoPartie = 80.0;
    if (chronoPartie < meilleursScores[TABLEAU_SCORES_TAILLE - 1].chrono) {
        char nomJoueur[5];
        printw("Bravo, vous avez battu un meilleur chrono ! Entrez votre nom (4 caractères) : ");
        refresh();
        getnstr(nomJoueur, 4);

        // Mettez à jour le tableau des meilleurs scores
        meilleursScores[TABLEAU_SCORES_TAILLE - 1].classement = 0;  // Indicateur que c'est un score du joueur
        strcpy(meilleursScores[TABLEAU_SCORES_TAILLE - 1].nom, nomJoueur);
        meilleursScores[TABLEAU_SCORES_TAILLE - 1].chrono = chronoPartie;

        // Triez le tableau des meilleurs scores
        trierMeilleursScores(meilleursScores, TABLEAU_SCORES_TAILLE);

        // Écrivez les trois meilleurs scores dans le fichier jeuhighscore.txt
        fichier = fopen(FICHIER_SCORES, "w");
        if (fichier) {
            for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
                if (meilleursScores[i].classement > 0) {
                    fprintf(fichier, "%d %4s %.1lf\n", i + 1, meilleursScores[i].nom, meilleursScores[i].chrono);
                }
            }
            fclose(fichier);
        }
    }

    // Affichage des meilleurs scores
    printw("Meilleurs Scores :\n");
    for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
        if (meilleursScores[i].classement > 0) {
            printw("%d. %4s %.1lf\n", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
        }
    }
    refresh();

    getch();  // Attendez que l'utilisateur appuie sur une touche
    endwin();
    return 0;
}
