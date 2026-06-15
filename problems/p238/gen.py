"""Tests for p238 (Uncle Vasya and Bags for Potatoes): N <= 18 bags
forming a forest (each bag is inside at most one bag, the rest lie on
the floor); line i lists Ci and the bags directly inside bag i.

Seed 1 is the single bag, seed 2 the maximal N=18 chain, seed 3 all 18
bags on the floor, seed 4 a star (one bag holding all others), the rest
random forests of random size.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, parent):
    children = [[] for _ in range(n + 1)]
    for v in range(1, n + 1):
        if parent[v]:
            children[parent[v]].append(v)
    print(n)
    for v in range(1, n + 1):
        print(len(children[v]), *children[v])


if seed == 1:
    emit(1, [0, 0])
elif seed == 2:
    # Chain: bag 1 contains bag 2 contains ... contains bag 18.
    emit(18, [0, 0] + list(range(1, 18)))
elif seed == 3:
    emit(18, [0] * 19)
elif seed == 4:
    emit(18, [0, 0] + [1] * 17)
else:
    n = rng.randint(2, 18)
    order = list(range(1, n + 1))
    rng.shuffle(order)
    parent = [0] * (n + 1)
    for i, v in enumerate(order):
        if i > 0 and rng.random() < 0.7:
            parent[v] = order[rng.randrange(i)]
    emit(n, parent)
