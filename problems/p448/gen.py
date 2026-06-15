"""Tests for p448 (Controlled Tournament): count min-height tournament
trees won by player M.

N <= 16, win matrix R with R[i][j] = 1 - R[j][i] off the diagonal.
Seed 1 is N=1. Seeds 2-3 are N=16: a random tournament where the friend
beats roughly half the field, and a "near-champion" friend who loses to
exactly one player. Later seeds cover N=15 (uneven tree), small N,
a friend who loses to everyone, and a total order (transitive matrix).
"""

import random
import sys


def emit(n, m, beats):
    print(n, m + 1)
    for i in range(n):
        print(' '.join('1' if i != j and beats[i][j] else '0'
                       for j in range(n)))


def random_matrix(n, rng):
    beats = [[False] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            w = rng.random() < 0.5
            beats[i][j] = w
            beats[j][i] = not w
    return beats


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, 0, [[False]])
elif seed == 2:
    n = 16
    emit(n, rng.randrange(n), random_matrix(n, rng))
elif seed == 3:
    # friend beats everyone except one nemesis; rest random
    n = 16
    beats = random_matrix(n, rng)
    m = rng.randrange(n)
    nemesis = rng.choice([x for x in range(n) if x != m])
    for j in range(n):
        if j != m:
            beats[m][j] = (j != nemesis)
            beats[j][m] = (j == nemesis)
    emit(n, m, beats)
elif seed == 4:
    # N=15: minimal-height tree is uneven (subtrees of sizes 7+8 etc.)
    n = 15
    emit(n, rng.randrange(n), random_matrix(n, rng))
elif seed == 5:
    # transitive total order, friend is the strongest -> all trees count
    n = 16
    order = list(range(n))
    rng.shuffle(order)
    beats = [[False] * n for _ in range(n)]
    for a in range(n):
        for b in range(a + 1, n):
            beats[order[a]][order[b]] = True
    emit(n, order[0], beats)
elif seed == 6:
    # friend loses to everyone -> 0 unless N == 1
    n = 16
    beats = random_matrix(n, rng)
    m = rng.randrange(n)
    for j in range(n):
        if j != m:
            beats[m][j] = False
            beats[j][m] = True
    emit(n, m, beats)
else:
    n = rng.randint(2, 9)
    emit(n, rng.randrange(n), random_matrix(n, rng))
