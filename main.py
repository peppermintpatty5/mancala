#!/usr/bin/env python3

from mancala import Mancala


if __name__ == "__main__":
    m = Mancala.start()

    while not m.game_over():
        print(m)
        p = 1 if m.p1_turn else 2
        i = int(input(f"P{p}> "))
        next = m.move(i - 1)

        if next is not None:
            m = next
        else:
            print("Invalid move")
