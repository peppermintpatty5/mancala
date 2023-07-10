#include <stdio.h>
#include <stdlib.h>

#include "mancala.h"

int main(void)
{
    struct mancala m;

    mancala_init(&m);

    while (1)
    {
        int i;

        mancala_print(&m, stdout);
        printf("Player %i's turn: ", m.state + 1);

        do
        {
            scanf("%i", &i);
        } while (i < 0 || i >= NUM_CUPS);

        mancala_do_turn(&m, i);
    }

    return EXIT_SUCCESS;
}
