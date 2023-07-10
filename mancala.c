#include "mancala.h"

void mancala_init(struct mancala *game)
{
    static const struct mancala starting_state = {
        TURN_PLAYER_1,
        {{4, 4, 4, 4, 4, 4},
         {4, 4, 4, 4, 4, 4}},
        {0, 0}};

    *game = starting_state;
}

int mancala_do_turn(struct mancala *game, int index)
{
    if (game->state != GAME_OVER)
    {
        unsigned short x,
            *near = game->cups[game->state],
            *far = game->cups[!game->state],
            *score = &game->scores[game->state];

        if (index >= 0 && index < NUM_CUPS && (x = near[index]) > 0)
        {
            int i;
            unsigned short q = x / (NUM_CUPS * 2 + 1),
                           r = x % (NUM_CUPS * 2 + 1);

            /* distribute stones to cups */
            near[index] = 0;

            for (i = 0; i < NUM_CUPS; i++)
            {
                int dist = i > index
                               ? i - index - 1
                               : i - index + NUM_CUPS * 2;

                near[i] += q + (dist < r);
            }

            for (i = 0; i < NUM_CUPS; i++)
                far[i] += q + (i - index + NUM_CUPS < r);

            *score += q + ((NUM_CUPS - index - 1) < r);

            /* opposite cup stealing */
            if (index + r >= 0 && index + r < NUM_CUPS)
            {
                unsigned short
                    *last_cup = &near[index + r],
                    *opposite = &far[NUM_CUPS - (index + r) - 1];

                if (*last_cup == 1 && *opposite > 0)
                {
                    *score += 1 + *opposite;
                    *last_cup = *opposite = 0;
                }
            }

            /* go again if last stone dropped in mancala */
            if (index + r != NUM_CUPS)
            {
                game->state = game->state == TURN_PLAYER_1
                                  ? TURN_PLAYER_2
                                  : TURN_PLAYER_1;
            }
        }
    }

    return 0;
}

void mancala_print(struct mancala *game, FILE *out)
{
    int i;
    unsigned short *near = game->cups[game->state],
                   *far = game->cups[!game->state];

    fputs("+----+", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, "----+");
    fputs("----+\n", out);

    fputs("|    |", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, " %2hi |", far[NUM_CUPS - i - 1]);
    fputs("    |\n", out);

    fprintf(out, "| %2hi |", game->scores[!game->state]);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, "----+");
    fprintf(out, " %2hi |\n", game->scores[game->state]);

    fputs("|    |", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, " %2hi |", near[i]);
    fputs("    |\n", out);

    fputs("+----+", out);
    for (i = 0; i < NUM_CUPS; i++)
        fprintf(out, "----+");
    fputs("----+\n", out);
}
