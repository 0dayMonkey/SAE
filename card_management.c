#include "header.h"

// Initializes the array with pairs of matching values and shuffles them.
void init_card(int values[PAIR_COUNT * 2]) {
    // Assigning pairs of matching values to the array
    for (int i = 0; i < PAIR_COUNT; ++i) {
        values[i * 2] = values[i * 2 + 1] = i + 1;
    }

    // Shuffling the array to randomize card positions
    for (int i = 0; i < PAIR_COUNT * 2; ++i) {
        int r = rand() % (PAIR_COUNT * 2);
        int temp = values[i];
        values[i] = values[r];
        values[r] = temp;
    }
}

// Positions card windows on the screen based on start coordinates.
void position_cards(WINDOW* card_wins[3][4], int start_y, int start_x, int revealed[3][4], int debug) {
    // Creating windows for each card and positioning them
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            card_wins[i][j] = newwin(CARD_HEIGHT, CARD_WIDTH,
                                     start_y + i * (CARD_HEIGHT + PADDING),
                                     start_x + j * (CARD_WIDTH + PADDING));
            // Highlighting the first card window and drawing cards
            if (i == 0 && j == 0) {
                wattron(card_wins[i][j], COLOR_PAIR(1));
                draw_card(card_wins[i][j], revealed[i][j], ' ', debug);
                wattroff(card_wins[i][j], COLOR_PAIR(1));
            } else {
                draw_card(card_wins[i][j], revealed[i][j], ' ', debug);
            }
        }
    }
}

// Assigns shuffled values to cards and creates new windows for each card.
void assign_shuffled_values(int card_values[3][4], int values[], WINDOW* card_wins[3][4], int start_y, int start_x) {
    // Distributing values to each card and creating new windows
    for (int i = 0, k = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++, k++) {
            card_values[i][j] = values[k];
            card_wins[i][j] = newwin(CARD_HEIGHT, CARD_WIDTH,
                                     start_y + (CARD_HEIGHT + PADDING) * i,
                                     start_x + (CARD_WIDTH + PADDING) * j);
        }
    }
}

// Refreshes the display of all cards based on their current state.
void refresh_cards(WINDOW* card_wins[3][4], int revealed[3][4], int card_values[3][4], int selected[3][4]) {
    // Redrawing each card with its current state
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            int is_matched = selected[i][j];
            draw_card(card_wins[i][j], revealed[i][j], card_values[i][j], is_matched);
        }
    }
}

// Performs a random selection of a card position.
bool SelectionRandom(clock_t debut, int* current_y, int* current_x, int selected[3][4], int first_pick_y, int first_pick_x) {
    // Randomly moving to a new card position
    int random_move = rand() % 12 + 1;
    for (int i = 0; i < random_move; i++) {
        movecard(current_y, current_x, 1, selected, first_pick_y, first_pick_x);
    }
    return true;
}
