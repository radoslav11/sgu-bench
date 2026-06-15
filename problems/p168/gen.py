"""Tests for p168 (Matrix): N, M <= 1000, |A[i][j]| <= 32000.

Seed 1 is a 1x1 matrix. Seeds 2-3 are maximal 1000x1000 (uniform random
vs. a decreasing gradient that keeps the diagonal minima changing).
Later seeds cover extreme values, constant matrices, single row/column,
and random rectangular shapes.
"""

import random
import sys


def emit(n, m, gen_cell):
    out = [f"{n} {m}"]
    for i in range(n):
        out.append(" ".join(str(gen_cell(i, j)) for j in range(m)))
    sys.stdout.write("\n".join(out) + "\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, 1, lambda i, j: rng.randint(-32000, 32000))
elif seed == 2:
    emit(1000, 1000, lambda i, j: rng.randint(-32000, 32000))
elif seed == 3:
    # values decrease toward the bottom-right: minima always update
    emit(1000, 1000, lambda i, j: 32000 - 16 * (i + j) + rng.randint(0, 15))
elif seed == 4:
    emit(1000, 1000, lambda i, j: -32000)
elif seed == 5:
    emit(1, 1000, lambda i, j: rng.randint(-32000, 32000))
elif seed == 6:
    emit(1000, 1, lambda i, j: rng.randint(-32000, 32000))
elif seed == 7:
    # tiny value range: heavy ties
    emit(1000, 1000, lambda i, j: rng.randint(-1, 1))
else:
    n, m = rng.randint(1, 1000), rng.randint(1, 1000)
    emit(n, m, lambda i, j: rng.randint(-32000, 32000))
