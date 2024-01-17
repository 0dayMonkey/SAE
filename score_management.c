#include "header.h"

// Initializes or creates the score file with default content if it does not exist.
void initialize_score_file() {
    FILE* file = fopen("score.txt", "r");
    if (file == NULL) {
        // Creating and writing default content to the new score file
        file = fopen("score.txt", "w");
        fprintf(file, "1e:\n2e:\n3e:\n");
    }
    fclose(file);
}

// Updates the score file with a new entry (current score and pseudonym).
void update_score_file(float current_score, char *pseudonym) {
    float top_scores[3] = { 0 };
    char top_pseudonyms[3][5] = { { 0 } };
    char formatted_score[10];
    char line[20];

    // Reading current top scores and pseudonyms from the file
    FILE* file = fopen("score.txt", "r");
    if (file == NULL) {
        perror("Error while opening the score file");
        exit(-3);
    }

    for (int i = 0; i < 3; ++i) {
        if (fgets(line, sizeof(line), file) != NULL) {
            // Parsing line to extract score and pseudonym; reset if failed
            if (sscanf(line, "%*s %f %4s", &top_scores[i], top_pseudonyms[i]) != 2) {
                top_scores[i] = 0;
                strcpy(top_pseudonyms[i], "");
            }
        }
    }
    fclose(file);

    // Inserting the current score in the right order
    for (int i = 0; i < 3; ++i) {
        // Condition for inserting new score in the sorted list
        if (current_score < top_scores[i] || top_scores[i] == 0) {
            // Shifting scores below the new score
            for (int j = 2; j > i; --j) {
                top_scores[j] = top_scores[j - 1];
                strcpy(top_pseudonyms[j], top_pseudonyms[j - 1]);
            }
            // Placing the new score
            top_scores[i] = current_score;
            strncpy(top_pseudonyms[i], pseudonym, 4);
            top_pseudonyms[i][4] = '\0';
            break;
        }
    }

    // Writing updated scores back to the file
    file = fopen("score.txt", "w");
    if (file == NULL) {
        perror("Error while reopening the score file");
        exit(-3);
    }

    // Formatting and saving the updated scores
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
