#include <ncurses.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int classement;
    char nom[5];
    double chrono;
} Score;

// Fonction de tri pour les meilleurs scores (tri par chrono croissant)
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

    // Chargez les meilleurs scores depuis le fichier jeuhighscore.txt
    Score meilleursScores[3];
    FILE* fichier = fopen("jeuhighscore.txt", "r");
    if (fichier) {
        for (int i = 0; i < 3; i++) {
            if (fscanf(fichier, "%d %s %lf\n", &meilleursScores[i].classement,
                meilleursScores[i].nom, &meilleursScores[i].chrono) != 3) {
                printw("Erreur de lecture du fichier de scores.\n");
                refresh();
                getch();
                endwin();
                return 1;
            }
        }
        fclose(fichier);
    }
    else {
        // Le fichier de scores n'existe pas, initialisez les scores
        for (int i = 0; i < 3; i++) {
            meilleursScores[i].classement = i + 1;
            strcpy(meilleursScores[i].nom, "AAA");
            meilleursScores[i].chrono = 999.9;  // Une valeur élevée pour l'initialisation
        }
    }

    // Simulation d'une partie victorieuse avec un chrono de 80.0
    double chronoPartie = 80.0;
    if (chronoPartie < meilleursScores[2].chrono) {
        char nomJoueur[5];
        printw("Bravo, vous avez battu un meilleur chrono ! Entrez votre nom (4 caractères) : ");
        refresh();
        getnstr(nomJoueur, 4);

        // Mettez à jour le tableau des meilleurs scores
        strcpy(meilleursScores[2].nom, nomJoueur);
        meilleursScores[2].chrono = chronoPartie;

        // Triez le tableau des meilleurs scores
        trierMeilleursScores(meilleursScores, 3);

        // Écrivez les trois meilleurs scores dans le fichier jeuhighscore.txt
        FILE* fichier = fopen("jeuhighscore.txt", "w");
        if (fichier) {
            for (int i = 0; i < 3; i++) {
                fprintf(fichier, "%d %s %.1lf\n", i + 1, meilleursScores[i].nom, meilleursScores[i].chrono);
            }
            fclose(fichier);
        }
    }

    // Affichage des meilleurs scores
    printw("Meilleurs Scores :\n");
    for (int i = 0; i < 3; i++) {
        printw("%d. %s %.1lf\n", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
    }
    refresh();

    getch();  // Attendez que l'utilisateur appuie sur une touche
    endwin();
    return 0;
}
