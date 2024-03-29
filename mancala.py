import itertools
from typing import NamedTuple


class Mancala(NamedTuple):
    """
    Represents the state of a mancala game.
    """

    cups: tuple[tuple[int, int], ...]
    ends: tuple[int, int]
    p1_turn: bool

    def __str__(self) -> str:
        n = len(self.cups)
        top = "│".join(f" {c:2} " for _, c in self.cups)
        bot = "│".join(f" {c:2} " for c, _ in self.cups)

        return "\n".join(
            [
                "┌────┬────" + "┬────" * (n - 1) + "┬────┐",
                f"│ {self.ends[1]:2} │" + top + "│    │",
                "│    ├────" + "┼────" * (n - 1) + "┤    │",
                "│    │" + bot + f"│ {self.ends[0]:2} │",
                "└────┴────" + "┴────" * (n - 1) + "┴────┘",
            ]
        )

    @classmethod
    def start(cls) -> "Mancala":
        """
        Returns the starting state for a Mancala game, which is 6 pairs of cups
        with 4 stones in each.
        """
        return cls(tuple((4, 4) for _ in range(6)), (0, 0), True)

    def _near(self):
        """
        Returns a generator over the cups on the near side of the board, in the
        order from the first player's perspective.
        """
        return (c for c, _ in self.cups)

    def _far(self):
        """
        Returns a generator over the cups on the far side of the board, in the
        order from the second player's perspective.
        """
        return (c for _, c in reversed(self.cups))

    def game_over(self) -> bool:
        """
        A game is over if either player has no stones in any of their cups.
        """
        return all(c <= 0 for c, _ in self.cups) or all(c <= 0 for _, c in self.cups)

    def move(self, i_pick: int) -> "Mancala | None":
        """
        Performs a move for the player whose turn it is. `i_pick` is the index
        of the cup on the player's side to pick from and shall be in the range
        [0, n).

        If the selected cup is empty, then that move is not allowed an `None` is
        returned.
        """
        n = len(self.cups)
        x, _ = self.cups[i_pick]

        if x > 0:
            # distribute the stones around the board in a cycle
            q, r = divmod(x, 2 * n + 1)
            cycle = [
                q
                + (m if i != i_pick else 0)
                + (1 if (i - (i_pick + 1)) % (2 * n + 1) < r else 0)
                for i, m in enumerate(
                    itertools.chain(self._near(), (self.ends[0],), self._far())
                )
            ]

            near = cycle[:n]
            far_reversed = cycle[:n:-1]
            e0 = cycle[n]
            e1 = self.ends[1]

            # opposite cup stealing
            i_last = (i_pick + r) % (2 * n + 1)
            if i_last < n and near[i_last] == 1 and far_reversed[i_last] > 0:
                e0 += far_reversed[i_last] + 1
                near[i_last] = far_reversed[i_last] = 0

            # player goes again if last stone dropped in mancala
            return (
                Mancala(
                    cups=tuple(zip(near, far_reversed)),
                    ends=(e0, e1),
                    p1_turn=self.p1_turn,
                )
                if i_last == n
                else Mancala(
                    cups=tuple(zip(reversed(far_reversed), reversed(near))),
                    ends=(e1, e0),
                    p1_turn=not self.p1_turn,
                )
            )
