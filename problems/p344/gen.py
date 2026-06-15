"""Tests for p344 (Weed): N, M <= 1000 grid of 'X' / '.'.

Seed 1 is the 1x1 empty grid. Seed 2 is a full 1000x1000 grid with ~50%
weed (max BFS volume). Seed 3 is 1000x1000 with a sparse anti-diagonal
seeding that cascades into filling huge areas (long propagation chains).
Other seeds: all-X, all-empty, single row/column, sparse random dust,
and random densities.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, cell):
    print(n, m)
    for i in range(n):
        print("".join("X" if cell(i, j) else "." for j in range(m)))


if seed == 1:
    emit(1, 1, lambda i, j: False)
elif seed == 2:
    emit(1000, 1000, lambda i, j: rng.random() < 0.5)
elif seed == 3:
    # Diagonal stripes of single X's: each pair of diagonal neighbours
    # infects the cell between them, cascading across the whole board.
    emit(1000, 1000, lambda i, j: (i + j) % 2 == 0 and (i * 7 + j * 13) % 19 == 0)
elif seed == 4:
    emit(1000, 1000, lambda i, j: True)
elif seed == 5:
    emit(1000, 1000, lambda i, j: False)
elif seed == 6:
    emit(1, 1000, lambda i, j: rng.random() < 0.3)
elif seed == 7:
    emit(1000, 1, lambda i, j: rng.random() < 0.3)
elif seed == 8:
    # Very sparse dust on a big board.
    emit(1000, 1000, lambda i, j: rng.random() < 0.002)
elif seed == 9:
    # Diagonal main line: fills the bounding square.
    emit(1000, 1000, lambda i, j: i == j)
else:
    n = rng.randint(1, 60)
    m = rng.randint(1, 60)
    p = rng.random() * 0.6
    emit(n, m, lambda i, j: rng.random() < p)
