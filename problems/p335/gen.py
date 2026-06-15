"""Tests for p335 (Thiefs And Cops): H, W <= 5*10^8 grid, cop and thief
start on distinct cells, fourth line is 'C' or 'T' for who moves first.

Seed 1 is the smallest 1x2 grid, seed 2 the full 5*10^8 square with far
random positions, seed 3 a maximal one-column grid. Later seeds pin the
special cases: exact diagonal separation, opposite chessboard colors
(eternal evasion), adjacent starts, corners, and random small grids that
exercise every branch of the case analysis.
"""

import random
import sys

MAX = 500000000

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(h, w, rc, cc, rt, ct, mover):
    assert 1 <= rc <= h and 1 <= cc <= w
    assert 1 <= rt <= h and 1 <= ct <= w
    assert (rc, cc) != (rt, ct)
    print(h, w)
    print(rc, cc)
    print(rt, ct)
    print(mover)


def distinct_pair(h, w):
    while True:
        a = (rng.randint(1, h), rng.randint(1, w))
        b = (rng.randint(1, h), rng.randint(1, w))
        if a != b:
            return a, b


if seed == 1:
    emit(1, 2, 1, 1, 1, 2, "C")
elif seed == 2:
    emit(MAX, MAX, 17, 23, MAX - 5, MAX - 8, "C")
elif seed == 3:
    emit(MAX, 1, 1, 1, MAX, 1, "T")
elif seed == 4:
    emit(1, MAX, 1, MAX // 2, 1, MAX // 2 + 1, "C")
elif seed == 5:
    # Exact diagonal separation on a huge grid, same chessboard color.
    emit(MAX, MAX, 1000, 2000, 250000000, 250001000, "T")
elif seed == 6:
    # Opposite chessboard colors on a huge 2D grid: thief evades forever.
    emit(MAX, MAX - 1, 1, 1, MAX, MAX - 2, "C")
elif seed == 7:
    # Cop adjacent to the thief.
    h, w = rng.randint(2, 10), rng.randint(2, 10)
    rc, cc = rng.randint(1, h), rng.randint(1, w - 1)
    emit(h, w, rc, cc, rc, cc + 1, rng.choice("CT"))
elif seed == 8:
    # Both in opposite corners of a big near-square grid.
    h, w = MAX, MAX - 3
    emit(h, w, 1, 1, h, w, rng.choice("CT"))
else:
    shape = rng.choice(["tiny", "thin", "big"])
    if shape == "tiny":
        h, w = rng.randint(1, 6), rng.randint(1, 6)
        if h * w < 2:
            h, w = 2, 2
    elif shape == "thin":
        h, w = 1, rng.randint(2, MAX)
        if rng.random() < 0.5:
            h, w = w, h
    else:
        h, w = rng.randint(2, MAX), rng.randint(2, MAX)
    (rc, cc), (rt, ct) = distinct_pair(h, w)
    emit(h, w, rc, cc, rt, ct, rng.choice("CT"))
