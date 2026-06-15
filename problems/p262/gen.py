"""Tests for p262 (Symbol Recognition): N, M <= 10 screen, 2 <= K <= 6
unique non-empty symbols given as 0/1 grids separated by blank lines.

Seed 1 is minimal (1x2 screen, K=2). Seed 2 is maximal (10x10, K=6,
random symbols). Seed 3 is maximal with near-identical symbols (each a
small perturbation of a common base, so few distinguishing pixels).
The rest are random sizes and densities.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, symbols):
    blocks = ["\n".join("".join(map(str, row)) for row in s) for s in symbols]
    print(n, m, len(symbols))
    print("\n\n".join(blocks))


def random_symbol(n, m, density=0.5):
    while True:
        g = [[1 if rng.random() < density else 0 for _ in range(m)]
             for _ in range(n)]
        if any(any(row) for row in g):
            return g


def unique_symbols(n, m, k, make):
    seen = set()
    symbols = []
    while len(symbols) < k:
        g = make()
        key = tuple(map(tuple, g))
        if key not in seen:
            seen.add(key)
            symbols.append(g)
    return symbols


if seed == 1:
    emit(1, 2, [[[1, 0]], [[0, 1]]])
elif seed == 2:
    emit(10, 10, unique_symbols(10, 10, 6, lambda: random_symbol(10, 10)))
elif seed == 3:
    n, m = 10, 10
    base = random_symbol(n, m)

    def perturb():
        g = [row[:] for row in base]
        for _ in range(rng.randint(1, 2)):
            r, c = rng.randrange(n), rng.randrange(m)
            g[r][c] ^= 1
        if not any(any(row) for row in g):
            g[0][0] = 1
        return g

    emit(n, m, unique_symbols(n, m, 6, perturb))
else:
    n, m = rng.randint(1, 10), rng.randint(1, 10)
    k = rng.randint(2, 6)
    while 2 ** (n * m) - 1 < k:
        n, m = rng.randint(1, 10), rng.randint(1, 10)
    density = rng.choice([0.2, 0.5, 0.8])
    emit(n, m, unique_symbols(n, m, k, lambda: random_symbol(n, m, density)))
