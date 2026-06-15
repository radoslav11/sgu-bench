"""Tests for p216 (Royal Federation): split a tree into provinces of
size B..3B, each reaching its capital through its own cities.

N <= 10000, 1 <= B <= N. Trees are emitted with randomly permuted
vertex labels and randomly oriented edges. Seed 1 is the single-city
kingdom, seed 2 the maximal star with B = 1 (maximal number of
provinces and output size), seed 3 the maximal path with B = 3333
(province sizes pushed against the 3B bound and maximal recursion
depth). Later seeds: the sample, one province covering everything
(B = N), a caterpillar, a broom, a wide shallow tree and random trees
with random B.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, b, edges):
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    relabeled = []
    for u, v in edges:
        a, c = perm[u - 1], perm[v - 1]
        if rng.random() < 0.5:
            a, c = c, a
        relabeled.append((a, c))
    rng.shuffle(relabeled)
    out = [f"{n} {b}"]
    out += [f"{u} {v}" for u, v in relabeled]
    sys.stdout.write("\n".join(out) + "\n")


def path(n):
    return [(i, i + 1) for i in range(1, n)]


def star(n):
    return [(1, i) for i in range(2, n + 1)]


def random_tree(rng, n, skew):
    # parent of i drawn from the last `skew` vertices -> controls depth
    return [(rng.randint(max(1, i - skew), i - 1), i) for i in range(2, n + 1)]


if seed == 1:
    emit(1, 1, [])
elif seed == 2:
    emit(10000, 1, star(10000))
elif seed == 3:
    emit(10000, 3333, path(10000))
elif seed == 4:
    emit(8, 2, [(1, 2), (2, 3), (1, 8), (8, 7), (8, 6), (4, 6), (6, 5)])
elif seed == 5:
    emit(10000, 10000, random_tree(rng, 10000, 10000))
elif seed == 6:
    # caterpillar: spine of 5000 with a leaf on every spine city
    n = 10000
    spine = n // 2
    edges = path(spine)
    edges += [(i, spine + i) for i in range(1, n - spine + 1)]
    emit(n, 100, edges)
elif seed == 7:
    # broom: long handle ending in a huge bundle of leaves
    n = 10000
    handle = 5000
    edges = path(handle)
    edges += [(handle, i) for i in range(handle + 1, n + 1)]
    emit(n, 1666, edges)
elif seed == 8:
    # wide shallow tree, B = 2: many tiny provinces
    emit(10000, 2, random_tree(rng, 10000, 3))
else:
    n = rng.randint(2, 10000)
    b = rng.choice((1, 2, 3, rng.randint(1, n), max(1, n // 3),
                    max(1, n // 10)))
    emit(n, b, random_tree(rng, n, rng.choice((1, 2, 10, n))))
