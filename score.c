// SEULEMENT LORSQUE Q EST PRESSE OU TOUTES LES CARTES SONT RETOURNEES ET LE CHRONO S'ARRETE!
//SCORE DU JOUEUR COMPARER AU PIRE SCORE DU TABLEAU, SI MEILLEUR ALORS IL LE REPLACE PUIS LE TABLEAU EST TRIE, ENFIN LE CLASSEMENT 
//(CHIFFRE QUI PRECEDE CHAQUE JOUEUR EST MIS A JOUR POUR COORDONNER).
// SI FICHIER N'EXISTE PAS, ON RZJOUTE DES VALEURS PAR DEFAUT AVEC DES CHRONO MAX 999.999 AFIN DE POUVOIR JOUER





#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#define TABLEAU_SCORES_TAILLE 3
#define FICHIER_SCORES "jeuhighscore.txt"
#define TEMPS_JOUEUR 1.0

typedef struct {
    int classement;
    char nom[5]; // Augmentation de la taille pour inclure le caractère nul
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

void initialiserMeilleursScores(Score scores[], int n) {
    for (int i = 0; i < n; i++) {
        scores[i].classement = i + 1;
        strcpy(scores[i].nom, "AAA");
        scores[i].chrono = 999.9;
    }
}

int main() {
    initscr();
    noecho();
    cbreak();

    // Création de la fenêtre avec des bordures
    WINDOW *scoreWindow = newwin(TABLEAU_SCORES_TAILLE + 2, 25, 1, 1);
    box(scoreWindow, 0, 0);

    // Affichage de la fenêtre
    wrefresh(scoreWindow);

    Score meilleursScores[TABLEAU_SCORES_TAILLE];

    FILE *fichier = fopen(FICHIER_SCORES, "r");

    if (!fichier) {
        printw("Le fichier de scores n'existe pas. Création du fichier...\n");
        refresh();
        getch();

        // Initialiser les meilleurs scores avec des valeurs par défaut
        initialiserMeilleursScores(meilleursScores, TABLEAU_SCORES_TAILLE);

        // Écrire les valeurs par défaut dans le fichier
        fichier = fopen(FICHIER_SCORES, "w");
        if (fichier) {
            for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
                fprintf(fichier, "%d %4s %.1lf\n", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
            }
            fclose(fichier);
        }
    } else {
        // Charger les meilleurs scores depuis le fichier
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
    }

    double chronoPartie = TEMPS_JOUEUR;

    if (chronoPartie < meilleursScores[TABLEAU_SCORES_TAILLE - 1].chrono) {
        char nomJoueur[5];
        printw("Bravo, vous avez battu un meilleur chrono ! Entrez votre nom (4 caractères SEULEMENT!) : ");
        refresh();
        getnstr(nomJoueur,4);

        // Remplacer le dernier score du tableau
        meilleursScores[TABLEAU_SCORES_TAILLE - 1].classement = 0;
        strcpy(meilleursScores[TABLEAU_SCORES_TAILLE - 1].nom, nomJoueur);
        meilleursScores[TABLEAU_SCORES_TAILLE - 1].chrono = chronoPartie;

        // Trier le tableau
        trierMeilleursScores(meilleursScores, TABLEAU_SCORES_TAILLE);

        // Ajuster les classements
        ajusterClassements(meilleursScores, TABLEAU_SCORES_TAILLE);

        // Écrire les trois meilleurs scores dans le fichier
        fichier = fopen(FICHIER_SCORES, "w");
        if (fichier) {
            for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
                fprintf(fichier, "%d %4s %.1lf\n", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
            }
            fclose(fichier);
        }
    }

    // Affichage des scores dans la fenêtre avec des bordures
    mvwprintw(scoreWindow, 1, 1, "Meilleurs Scores :");
    for (int i = 0; i < TABLEAU_SCORES_TAILLE; i++) {
        mvwprintw(scoreWindow, i + 2, 1, "%d. %4s %.1lf", meilleursScores[i].classement, meilleursScores[i].nom, meilleursScores[i].chrono);
    }

    wrefresh(scoreWindow);

    getch();
    endwin();
    return 0;
}
