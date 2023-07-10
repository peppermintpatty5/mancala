#ifndef MANCALA_H_
#define MANCALA_H_

#include <stdio.h>

#define NUM_CUPS 6

struct mancala
{
    enum game_state
    {
        TURN_PLAYER_1,
        TURN_PLAYER_2,
        GAME_OVER
    } state;                          /* Current state of the game */
    unsigned short cups[2][NUM_CUPS]; /* Rows of cups for both players */
    unsigned short scores[2];         /* Each player's mancala */
};

/**
 * Initialize a mancala game to the starting state (4 stones in each cup).
 */
extern void mancala_init(struct mancala *game);

/**
 * Perform a move for the player whose turn it is.
 *
 * No moves are possible once the game is over.
 */
extern int mancala_do_turn(struct mancala *game, int index);

extern void mancala_print(struct mancala *game, FILE *out);

#endif
