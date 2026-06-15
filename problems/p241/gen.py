"""Tests for p241 (The United Fields of Chessboardia): count peaceful
placements of K rooks on the union of an NxN board at the origin and an
MxM board shifted by (W, H). 0 <= N, M <= 20, 0 <= W, H <= 10^9,
0 <= K <= 10^9. The answer is a big integer.

The input is always five numbers, so "maximal" means the largest DP:
seed 2 has two 20x20 boards partially overlapping with K at the rook
maximum, seed 3 has two disjoint far-apart 20x20 boards with K = 40.
Other seeds: the empty input, the samples, coinciding boards, K = 0,
a huge infeasible K, degenerate N = 0 / M = 0 boards, overlap in
columns but not rows, and random configurations.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0, 0, 0, 0, 0)
elif seed == 2:
    print(20, 20, 7, 13, 33)
elif seed == 3:
    print(20, 20, 10**9, 10**9, 40)
elif seed == 4:
    print(8, 2, 6, 8, 1)
elif seed == 5:
    print(8, 8, 3, 4, 1)
elif seed == 6:
    print(20, 20, 0, 0, 20)  # boards coincide
elif seed == 7:
    print(20, 20, 5, 5, 0)  # K = 0 -> exactly one placement
elif seed == 8:
    print(20, 20, 1, 1, 10**9)  # infeasible K -> 0
elif seed == 9:
    print(0, 20, 5, 7, 13)  # first board is empty
elif seed == 10:
    print(20, 0, 999999999, 0, 20)  # second board is empty
elif seed == 11:
    print(20, 20, 10, 10**9, 25)  # columns overlap, rows do not
else:
    n = rng.randint(0, 20)
    m = rng.randint(0, 20)
    w = rng.choice((rng.randint(0, 40), rng.randint(0, 10**9)))
    h = rng.choice((rng.randint(0, 40), rng.randint(0, 10**9)))
    k = rng.choice((rng.randint(0, 45), rng.randint(0, 10**9)))
    print(n, m, w, h, k)
