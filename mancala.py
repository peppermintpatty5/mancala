#!/usr/bin/env python3

from sys import argv
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
        return (c for c, _ in self.cups)

    def _far(self):
        return (c for _, c in reversed(self.cups))

    def move(self, index: int) -> "Mancala | None":
        n = len(self.cups)

        if 0 <= index < n:
            x, _ = self.cups[index]
            q, r = divmod(x, 2 * n + 1)

            near = tuple(
                q
                + (c if i != index else 0)
                + (1 if (i - index - 1) % (2 * n + 1) < r else 0)
                for i, c in enumerate(self._near())
            )
            far = tuple(
                q + c + (1 if i - index + n < r else 0)
                for i, c in enumerate(self._far())
            )
            e0 = q + self.ends[0] + (1 if index + r >= n else 0)
            e1 = self.ends[1]

            return Mancala(tuple(zip(near, reversed(far))), (e0, e1))


if __name__ == "__main__":
    m = Mancala.start()
    for arg in argv[1:]:
        next = m.move(int(arg))
        if next is None:
            break
        m = next
        print(m)
