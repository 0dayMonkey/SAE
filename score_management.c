#include "header.h"
///////////////////////// GESTION DES SCORES /////////////////////////

void initialize_score_file()
{
  FILE* file = fopen("score.txt", "r");
  if (file == NULL) {
    // File does not exist, create it and write initial content
    file = fopen("score.txt", "w");
    fprintf(file, "1e:\n2e:\n3e:\n");
  }
  fclose(file);
}
void update_score_file(float current_score, char* pseudonym)
{
  float top_scores[3] = { 0 };
  char top_pseudonyms[3][5] = { { 0 } };
  char formatted_score[10];
  char line[20];

  FILE* file = fopen("score.txt", "r");
  for (int i = 0; i < 3; ++i) {
    if (fgets(line, sizeof(line), file) != NULL) {
      // Vérifie si la ligne contient un score et un pseudonyme valides
      if (sscanf(line, "%*s %f %s", &top_scores[i], top_pseudonyms[i]) != 2) {
        top_scores[i] = 0;
        strcpy(top_pseudonyms[i], "");
      }
    }
  }
  fclose(file);

  // Insère le nouveau score à la bonne place
  for (int i = 0; i < 3; ++i) {
    if (current_score < top_scores[i] || top_scores[i] == 0) {
      for (int j = 2; j > i; --j) {
        top_scores[j] = top_scores[j - 1];
        strcpy(top_pseudonyms[j], top_pseudonyms[j - 1]);
      }
      top_scores[i] = current_score;
      strncpy(top_pseudonyms[i], pseudonym, 4);
      top_pseudonyms[i][4] = '\0';
      break;
    }
  }

  // Réécriture du fichier avec les nouveaux scores
  file = fopen("score.txt", "w");
  for (int i = 0; i < 3; ++i) {
    if (top_scores[i] > 0) {
      sprintf(formatted_score, "%.2f", top_scores[i]);
      fprintf(file, "%de: %ss %s\n", i + 1, formatted_score, top_pseudonyms[i]);
    } else {
      fprintf(file, "%de:\n", i + 1);
    }
  }
  fclose(file);
}

///////////////////////////////////////////////////////////////////////////
