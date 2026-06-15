"""Tests for p239 (Minesweeper): N <= 1000 numbers a_i in [0, 3], a_i is
the count of mines among first-column cells i-1, i, i+1; count the mine
distributions consistent with all a_i.

Seed 1 is N=1, seed 2 a maximal consistent N=1000 column derived from a
random mine layout, seed 3 maximal random a_i (usually answer 0),
seed 4 all zeros, seed 5 all mines, the rest mix consistent layouts,
small randoms, and consistent layouts with one perturbed value.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def from_mines(mines):
    n = len(mines)
    a = []
    for i in range(n):
        s = mines[i]
        if i > 0:
            s += mines[i - 1]
        if i + 1 < n:
            s += mines[i + 1]
        a.append(s)
    return a


def emit(a):
    print(len(a))
    for x in a:
        print(x)


if seed == 1:
    emit([0])
elif seed == 2:
    emit(from_mines([rng.randint(0, 1) for _ in range(1000)]))
elif seed == 3:
    emit([rng.randint(0, 3) for _ in range(1000)])
elif seed == 4:
    emit([0] * 1000)
elif seed == 5:
    emit(from_mines([1] * 1000))
else:
    n = rng.randint(1, 999)
    a = from_mines([rng.randint(0, 1) for _ in range(n)])
    if rng.random() < 0.5:
        i = rng.randrange(n)
        a[i] = rng.randint(0, 3)
    emit(a)
