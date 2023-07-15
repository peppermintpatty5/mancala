#include <stdio.h>
#include <stdlib.h>

#include "mancala.h"

int main(void)
{
    struct mancala game;

    mancala_init(&game);

    while (!mancala_game_over(&game))
    {
        int i;

        mancala_print(&game, stdout);
        printf("Player %i's turn: ", game.turn + 1);

        do
        {
            scanf("%i", &i);
        } while (i < 0 || i >= NUM_CUPS);

        mancala_do_turn(&game, i);
    }

    return EXIT_SUCCESS;
}
