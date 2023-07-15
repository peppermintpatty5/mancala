#ifndef MANCALA_H_
#define MANCALA_H_

#include <stdio.h>

#define NUM_CUPS 6

struct mancala
{
    int turn;
    struct player
    {
        unsigned short cups[NUM_CUPS];
        unsigned short mancala;
    } p1, p2;
};

/**
 * Create and initialize a mancala game to the usual starting state of 4 stones
 * in each cup.
 */
extern struct mancala *mancala_new(void);

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

/**
 * Print a representation of the game to the output stream.
 */
extern void mancala_print(struct mancala *game, FILE *out);

#endif
