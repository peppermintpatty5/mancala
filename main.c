#include <stdio.h>
#include <stdlib.h>

#include "mancala.h"

int main(void)
{
    struct mancala *game = mancala_new();

    while (!mancala_game_over(game))
    {
        int i;
        struct mancala *next;

        mancala_print(game, stdout);
        printf("Player %i's turn: ", game->turn + 1);

        do
        {
            scanf("%i", &i);
        } while ((next = mancala_do_turn(game, i - 1)) == NULL);

        free(game);
        game = next;
    }

    return EXIT_SUCCESS;
}
