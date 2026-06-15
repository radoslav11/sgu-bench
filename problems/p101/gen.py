"""Tests for p101 (Domino): chain N <= 100 pieces with digits 0..6.

Seed 1 is the single-piece minimum. Seed 2 is a maximal solvable test
(random walk on digits, then pieces shuffled and flipped). Seed 3 is a
maximal uniform-random test (usually unsolvable: too many odd degrees).
Later seeds mix solvable walks, doubles, and disconnected piece sets.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(pieces):
    pieces = list(pieces)
    rng.shuffle(pieces)
    print(len(pieces))
    for a, b in pieces:
        if rng.random() < 0.5:
            a, b = b, a
        print(a, b)


def walk_pieces(n, digits):
    cur = rng.choice(digits)
    pieces = []
    for _ in range(n):
        nxt = rng.choice(digits)
        pieces.append((cur, nxt))
        cur = nxt
    return pieces


if seed == 1:
    emit([(rng.randint(0, 6), rng.randint(0, 6))])
elif seed == 2:
    emit(walk_pieces(100, list(range(7))))
elif seed == 3:
    emit([(rng.randint(0, 6), rng.randint(0, 6)) for _ in range(100)])
elif seed == 4:
    # Two separate solvable chains on disjoint digit sets -> disconnected.
    emit(walk_pieces(40, [0, 1, 2]) + walk_pieces(40, [4, 5, 6]))
elif seed == 5:
    # Doubles only: (d, d) pieces never connect different digits.
    emit([(d, d) for d in range(7) for _ in range(rng.randint(1, 14))])
elif seed == 6:
    # All pieces identical.
    emit([(3, 5)] * 100)
elif seed == 7:
    # Solvable walk on two digits plus doubles hanging on the walk.
    pieces = walk_pieces(60, [2, 6])
    pieces += [(2, 2)] * 20 + [(6, 6)] * 20
    emit(pieces)
else:
    emit(walk_pieces(rng.randint(2, 99), rng.sample(range(7), rng.randint(2, 7))))
