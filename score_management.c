#include "header.h"


void initialize_score_file()
{
  FILE* file = fopen("score.txt", "r");
  if (file == NULL) {
    // Create file if it doesn't exist. Initial content sets up an empty score board.
    file = fopen("score.txt", "w");
    fprintf(file, "1e:\n2e:\n3e:\n");
  }
  fclose(file);
}




void update_score_file(float current_score, char* pseudonym) {
  float top_scores[3] = { 0 };
  char top_pseudonyms[3][5] = { { 0 } };
  char formatted_score[10];
  char line[20];

  // Open the score file in read mode
  FILE* file = fopen("score.txt", "r");
  if (file == NULL) {
    perror("Error opening score file");
    return;
  }

  // Reading existing scores and pseudonyms from the file
  for (int i = 0; i < 3; ++i) {
    if (fgets(line, sizeof(line), file) != NULL) {
      // Parse the line for score and pseudonym
      if (sscanf(line, "%*s %f %4s", &top_scores[i], top_pseudonyms[i]) != 2) {
        top_scores[i] = 0;
        strcpy(top_pseudonyms[i], "");
      }
    }
  }
  fclose(file);

  // Insert new score in the correct order
  for (int i = 0; i < 3; ++i) {
    if (current_score < top_scores[i] || top_scores[i] == 0) {
      // Shift lower scores down
      for (int j = 2; j > i; --j) {
        top_scores[j] = top_scores[j - 1];
        strcpy(top_pseudonyms[j], top_pseudonyms[j - 1]);
      }
      // Insert new score
      top_scores[i] = current_score;
      strncpy(top_pseudonyms[i], pseudonym, 4);
      top_pseudonyms[i][4] = '\0';
      break;
    }
  }

  // Write the updated scores back to the file
  file = fopen("score.txt", "w");
  if (file == NULL) {
    perror("Error reopening score file");
    return;
  }

  // Formatting and saving new scores
  for (int i = 0; i < 3; ++i) {
    if (top_scores[i] > 0) {
      sprintf(formatted_score, "%.2f", top_scores[i]);
      fprintf(file, "%de: %s %s\n", i + 1, formatted_score, top_pseudonyms[i]);
    } else {
      fprintf(file, "%de:\n", i + 1);
    }
  }
  fclose(file);
}




