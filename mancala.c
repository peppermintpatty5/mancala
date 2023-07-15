#include "mancala.h"

void mancala_init(struct mancala *game)
{
    static const struct mancala starting_state = {
        0,
        {{4, 4, 4, 4, 4, 4}, 0},
        {{4, 4, 4, 4, 4, 4}, 0}};

    *game = starting_state;
}

int mancala_do_turn(struct mancala *game, int index)
{
    unsigned short x;
    struct player *near = game->turn == 0 ? &game->p1 : &game->p2,
                  *far = game->turn == 0 ? &game->p2 : &game->p1;

    if (index >= 0 && index < NUM_CUPS && (x = near->cups[index]) > 0)
    {
        int i;
        unsigned short q = x / (NUM_CUPS * 2 + 1),
                       r = x % (NUM_CUPS * 2 + 1);

        /* distribute stones to cups */
        near->cups[index] = 0;

        for (i = 0; i < NUM_CUPS; i++)
        {
            int dist = i > index
                           ? i - index - 1
                           : i - index + NUM_CUPS * 2;

            near->cups[i] += q + (dist < r);
        }

        for (i = 0; i < NUM_CUPS; i++)
            far->cups[i] += q + (i - index + NUM_CUPS < r);

        near->mancala += q + ((NUM_CUPS - index - 1) < r);

        /* opposite cup stealing */
        if (index + r >= 0 && index + r < NUM_CUPS)
        {
            unsigned short
                *last_cup = &near->cups[index + r],
                *opposite = &far->cups[NUM_CUPS - (index + r) - 1];

            if (*last_cup == 1 && *opposite > 0)
            {
                near->mancala += 1 + *opposite;
                *last_cup = *opposite = 0;
            }
        }

        /* go again if last stone dropped in mancala */
        if (index + r != NUM_CUPS)
        {
            game->turn = !game->turn;
        }

        return 1;
    }

    return 0;
}

static int all_zero(unsigned short cups[])
{
    int i = 0;

    for (i = 0; i < NUM_CUPS; i++)
    {
        if (cups[i] != 0)
            return 0;
    }
    return 1;
}

int mancala_game_over(struct mancala *game)
{
    return all_zero(game->p1.cups) || all_zero(game->p2.cups);
}

void mancala_print(struct mancala *game, FILE *out)
{
    int i;
    struct player *near = game->turn == 0 ? &game->p1 : &game->p2,
                  *far = game->turn == 0 ? &game->p2 : &game->p1;

    fputs("+----+", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, "----+");
    fputs("----+\n", out);

    fputs("|    |", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, " %2hi |", far->cups[NUM_CUPS - i - 1]);
    fputs("    |\n", out);

    fprintf(out, "| %2hi |", far->mancala);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, "----+");
    fprintf(out, " %2hi |\n", near->mancala);

    fputs("|    |", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, " %2hi |", near->cups[i]);
    fputs("    |\n", out);

    fputs("+----+", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, "----+");
    fputs("----+\n", out);
}
