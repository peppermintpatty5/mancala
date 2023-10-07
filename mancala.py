#!/usr/bin/env python3

import itertools
import sys
from typing import NamedTuple


class Mancala(NamedTuple):
    cups: tuple[tuple[int, int], ...]
    ends: tuple[int, int]

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
        return cls(tuple((4, 4) for _ in range(6)), (0, 0))

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

    def move(self, i_pick: int) -> "Mancala | None":
        n = len(self.cups)

        if 0 <= i_pick < n:
            x, _ = self.cups[i_pick]
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
            far = cycle[n + 1 :]
            e0 = cycle[n]
            e1 = self.ends[1]

            return Mancala(tuple(zip(near, reversed(far))), (e0, e1))


if __name__ == "__main__":
    m = Mancala.start()
    for arg in sys.argv[1:]:
        next = m.move(int(arg))
        if next is None:
            break
        m = next
        print(m)
