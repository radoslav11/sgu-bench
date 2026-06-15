"""Tests for p149 (Computer Network): weighted tree, N <= 10000,
total cable length <= 10^9. Computer i (2..N) attaches to an earlier one.

Seed 1 is the minimal tree (N=1). Seeds 2-3 are maximal: a path with the
largest weights (deepest recursion) and a random tree with large weights.
Later seeds cover stars, caterpillars, balanced binary shapes and ties.
"""

import random
import sys

MAXN = 10000
TOTAL = 10**9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(parents, weights):
    n = len(parents) + 1
    print(n)
    for p, w in zip(parents, weights):
        print(p, w)


def rand_weights(m, total):
    # m positive weights with sum <= total, most near the per-edge cap
    cap = total // max(m, 1)
    return [rng.randint(max(1, cap // 2), cap) for _ in range(m)]


if seed == 1:
    print(1)
elif seed == 2:
    # maximal path: depth N-1, big weights
    parents = list(range(1, MAXN))
    emit(parents, rand_weights(MAXN - 1, TOTAL))
elif seed == 3:
    # maximal random tree, big weights
    parents = [rng.randint(1, i) for i in range(1, MAXN)]
    emit(parents, rand_weights(MAXN - 1, TOTAL))
elif seed == 4:
    # star: everything hangs off computer 1
    parents = [1] * (MAXN - 1)
    emit(parents, rand_weights(MAXN - 1, TOTAL))
elif seed == 5:
    # caterpillar: long spine with leaves attached, unit-ish weights
    parents = []
    spine = [1]
    for i in range(2, MAXN + 1):
        if i % 2 == 0:
            parents.append(spine[-1])
            spine.append(i)
        else:
            parents.append(spine[rng.randrange(len(spine))])
    emit(parents, [rng.randint(1, 3) for _ in range(MAXN - 1)])
elif seed == 6:
    # complete-ish binary tree, equal weights (many ties in eccentricity)
    parents = [i // 2 for i in range(2, MAXN + 1)]
    emit(parents, [7] * (MAXN - 1))
elif seed == 7:
    # two long chains from the root: diameter passes through node 1
    n = MAXN
    parents = []
    last_a, last_b = 1, 1
    for i in range(2, n + 1):
        if i % 2 == 0:
            parents.append(last_a)
            last_a = i
        else:
            parents.append(last_b)
            last_b = i
    emit(parents, rand_weights(n - 1, TOTAL))
else:
    # random mid-size trees with random weights
    n = rng.randint(2, 3000)
    parents = [rng.randint(1, i) for i in range(1, n)]
    emit(parents, [rng.randint(1, 1000) for _ in range(n - 1)])
