#ifndef MANCALA_H_
#define MANCALA_H_

#include <stdio.h>

#define NUM_CUPS 6

struct mancala
{
    int turn;                         /* Which player's turn it is */
    unsigned short cups[2][NUM_CUPS]; /* Rows of cups for both players */
    unsigned short scores[2];         /* Each player's mancala */
};

/**
 * Initialize a mancala game to the starting state (4 stones in each cup).
 */
extern void mancala_init(struct mancala *game);

/**
 * Perform a move for the player whose turn it is.
 */
extern int mancala_do_turn(struct mancala *game, int index);

/**
 * Return non-zero if the given game is over.
 *
 * A game is over if either player has no more stones in their row of cups.
 */
extern int mancala_game_over(struct mancala *game);

extern void mancala_print(struct mancala *game, FILE *out);

#endif
