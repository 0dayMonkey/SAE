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

///////////////////////// score_management.c //////////////////////////////

/*****************************************************************************
 * @brief   Initializes the score file for the game.
 *          It checks if the score file exists. If not, it creates the file
 *          and initializes it with default content.
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     None
 * @note    -
 *
 *****************************************************************************/
void initialize_score_file();

/*****************************************************************************
 * @brief   Updates the score file with the current player's score.
 *          It reads the existing scores from the file and inserts the new score
 *          in the correct position, ensuring the top three scores are recorded.
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     current_score - The score of the current player.
 *          pseudonym - The pseudonym of the current player.
 * @note    Assumes the existence of "score.txt". Handles file errors with perror.
 *****************************************************************************/
void update_score_file(float current_score, char *pseudonym);



///////////////////////// time_management.c ///////////////////////////////

/*****************************************************************************
 * @brief   Calculates the elapsed time in seconds since the given start time.
 *          It also displays this time on the screen at a specified position.
 * @author  Naïm
 * @date    
 * @return  float - The number of seconds elapsed since the start time.
 * @arg     debut - The start time in clock ticks.
 *          colonne - The column position for displaying the time.
 *          ligne - The line position for displaying the time.
 * @note    This function uses ncurses library functions for display.
 *****************************************************************************/
float chrono(clock_t debut, int colonne, int ligne);

/*****************************************************************************
 * @brief   Waits for a specified number of seconds.
 *          It calculates the elapsed time and checks if it is less than
 *          the specified number of seconds.
 * @author  Naïm
 * @date    
 * @return  int - 1 (true) if the elapsed time is less than the specified
 *          duration, 0 (false) otherwise.
 * @arg     nbsec - The number of seconds to wait.
 *          chrono_debut_lapsedeteemps - The elapsed time at the start of waiting.
 * @note    Relies on the chrono function for time calculations.
 *****************************************************************************/
int waiting(int nbsec, float chrono_debut_lapsedeteemps);



///////////////////////// visual_management.c //////////////////////////////

/*****************************************************************************
 * @brief   Draws a card in a specified window with its current state.
 *          The card can be in a matched state, revealed state, or hidden state.
 *          The appearance changes accordingly.
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     card_win - A pointer to the window where the card is to be drawn.
 *          revealed - A flag indicating if the card's value is revealed.
 *          value - The value of the card to display if revealed or matched.
 *          matched - A flag indicating if the card has been successfully matched.
 * @note    -
 *****************************************************************************/
void draw_card(WINDOW *card_win, int revealed, int value, int matched);

/*****************************************************************************
 * @brief   Creates and displays a welcome message in a window at the top of the screen.
 *          It calculates the size of the window based on the current screen size
 *          and centers the welcome message within it.
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     None
 * @note    -
 *****************************************************************************/
void TextBox();



///////////////////////// check_management.c //////////////////////////////

/*****************************************************************************
 * @brief   Moves the cursor to a new card position based on the given direction.
 *          It accounts for wrapping around the grid and already selected cards.
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     y - Pointer to the current vertical position of the cursor.
 *          x - Pointer to the current horizontal position of the cursor.
 *          direction - The direction to move (-1 for left, 1 for right).
 *          selected - Array indicating which cards have been matched.
 *          first_pick_y - The vertical position of the first selected card.
 *          first_pick_x - The horizontal position of the first selected card.
 * @note    Ensures that the new position is valid and not already selected. 
 *          Used for the "Auto" playing mod
 *****************************************************************************/
void movecard(int *y, int *x, int direction, int selected[3][4], int first_pick_y, int first_pick_x);

/*****************************************************************************
 * @brief   Checks if the game should end based on the current state.
 *          It determines if all cards are matched, or if a forced end or
 *          quit command is given. Displays scores and exits the game accordingly.
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     card_wins - Array of window pointers for each card.
 *          selected - Array indicating which cards have been matched.
 *          autom - Flag to indicate if the game is in automatic mode.
 *          force_end - Flag to indicate if the game should end forcefully.
 *          quitbyq - Flag to indicate if the game was quit by pressing 'q'.
 * @note    Window handling and I/O (score name input). Assumes a "score.txt" file.
 *****************************************************************************/
void checkend(WINDOW *card_wins[3][4], int selected[3][4], int autom, int force_end, int quitbyq);

/*****************************************************************************
 * @brief   Checks if the terminal window size is sufficient for the game.
 *          It exits the game with an error message if the window size is 
 *          too small (min 170x60 actualy).
 * @author  Naïm
 * @date    
 * @return  void
 * @arg     None
 * @note    Uses ncurses for checking window with getmaxx.
 *****************************************************************************/
void checkWindowSize();


/////////////////////////// jeu.c ////////////////////////////////

/*****************************************************************************
 * @brief   Performs a random selection of a card position.
 *          Moves to a new card position a random number of times.
 * @author  Naïm
 * @date    
 * @return  bool - Always returns true, indicating a selection was made.
 * @arg     debut - The start time used for timing (unused in this function).
 *          current_y - Pointer to the current vertical position of the cursor.
 *          current_x - Pointer to the current horizontal position of the cursor.
 *          selected - Array indicating which cards have been matched.
 *          first_pick_y - The vertical position of the first selected card.
 *          first_pick_x - The horizontal position of the first selected card.
 * @note    Uses the movecard function to navigate through the card positions.
 *****************************************************************************/
bool SelectionRandom(clock_t debut, int *current_y, int *current_x, int selected[3][4], int first_pick_y, int first_pick_x);

/*****************************************************************************
 * @brief   Main game function for Paire ou Perd.
 *          Handles game initialization, gameplay loop, user interactions,
 *          card shuffling and revealing, and game termination conditions.
 * @author  Naïm
 * @date    
 * @return  int - Returns 0 upon successful completion of the game.
 * @arg     autom - Flag to indicate if the game is in automatic mode.
 * @note    Utilizes ncurses for graphical display and interaction.
 *          Manages game state, card positions, and handles user inputs.
 *          Also controls game termination and score management.
 *****************************************************************************/
int jeu(int autom);


#endif // HEADER_H_INCLUDED
