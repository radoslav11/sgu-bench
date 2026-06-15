"""Tests for p280 (Trade centers): tree, N <= 30000, 1 <= K <= 100.

Seed 1 is the single-city case, seeds 2-3 are maximal (path with K=1 and
a deep-ish random tree with K=100), later seeds mix shapes: stars,
caterpillars, binary trees, random K.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 30000


def emit(n, k, edges):
    out = [f"{n} {k}"]
    out += [f"{u} {v}" for u, v in edges]
    print("\n".join(out))


if seed == 1:
    emit(1, 1, [])
elif seed == 2:
    # max path, K=1: many centers, deep recursion
    emit(MAXN, 1, [(i, i + 1) for i in range(1, MAXN)])
elif seed == 3:
    # max random tree with limited width (long), K=100
    edges = [(rng.randint(max(1, i - 40), i - 1), i) for i in range(2, MAXN + 1)]
    emit(MAXN, 100, edges)
elif seed == 4:
    # max shallow random tree, K=2
    edges = [(rng.randint(1, i - 1), i) for i in range(2, MAXN + 1)]
    emit(MAXN, 2, edges)
elif seed == 5:
    # caterpillar: spine of 15000 with one leg each, K=3
    n = MAXN
    spine = n // 2
    edges = [(i, i + 1) for i in range(1, spine)]
    edges += [(rng.randint(1, spine), i) for i in range(spine + 1, n + 1)]
    emit(n, 3, edges)
elif seed == 6:
    # complete binary tree, K random small
    n = MAXN
    edges = [(i // 2, i) for i in range(2, n + 1)]
    emit(n, rng.randint(1, 5), edges)
elif seed == 7:
    # star: K large, one center suffices
    n = rng.randint(2, 1000)
    emit(n, 100, [(1, i) for i in range(2, n + 1)])
else:
    # random mid-size tree, random K
    n = rng.randint(2, 5000)
    k = rng.randint(1, 100)
    edges = [(rng.randint(1, i - 1), i) for i in range(2, n + 1)]
    emit(n, k, edges)
