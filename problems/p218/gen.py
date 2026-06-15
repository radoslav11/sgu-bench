"""Tests for p218 (Unstable Systems): bottleneck assignment on an
n x n matrix, n <= 500, |values| <= 10^6.

Seed 1 is the single-workstation case, seed 2 a maximal uniform random
matrix, seed 3 a maximal matrix with few distinct values (many ties, so
the binary search sees fat equality groups and matchings are highly
ambiguous). Later seeds: the sample, an anti-greedy diagonal pattern
where each row's cheapest column collides, negative-heavy matrices and
random mid-size matrices.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, matrix):
    out = [str(n)]
    out += [" ".join(str(x) for x in row) for row in matrix]
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, [[-1000000]])
elif seed == 2:
    n = 500
    emit(n, [[rng.randint(-10**6, 10**6) for _ in range(n)] for _ in range(n)])
elif seed == 3:
    n = 500
    emit(n, [[rng.choice((-5, 0, 3, 7)) for _ in range(n)] for _ in range(n)])
elif seed == 4:
    emit(2, [[1, 3], [4, 5]])
elif seed == 5:
    # every row prefers column 0; bottleneck forced onto larger entries
    n = 500
    matrix = [[10**6] * n for _ in range(n)]
    for i in range(n):
        matrix[i][0] = -10**6
        matrix[i][(i * 7 + 3) % n] = i - 250
    emit(n, matrix)
elif seed == 6:
    # all values equal: answer is that value, any permutation works
    n = 500
    emit(n, [[123456] * n for _ in range(n)])
elif seed == 7:
    n = 500
    emit(n, [[rng.randint(-10**6, -10**6 + 10) for _ in range(n)]
             for _ in range(n)])
else:
    n = rng.randint(2, 400)
    span = rng.choice((2, 100, 10**6))
    emit(n, [[rng.randint(-span, span) for _ in range(n)] for _ in range(n)])
