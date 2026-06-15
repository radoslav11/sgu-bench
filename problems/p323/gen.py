"""Tests for p323 (Aviamachinations).

N towns (<= 2000), M airlines (<= 2000), K flights (<= 200000); each
flight is "a b c p" with a != b, airline c, tax p <= 100000. The whole
flight graph must be connected. A spanning tree over a random vertex
permutation guarantees that; extra flights are sprinkled on top.

Seed 1 is the minimal one-town case, seeds 2-3 are maximal (uniform
random vs. clustered taxes / few dominant airlines), later seeds add
ties, single-edge airlines and an airline that already spans everything.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, flights):
    print(n, m, len(flights))
    out = []
    for a, b, c, p in flights:
        out.append(f"{a} {b} {c} {p}")
    print("\n".join(out))


def spanning_tree(n, rng):
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    edges = []
    for i in range(1, n):
        j = rng.randrange(max(0, i - 8), i) if rng.random() < 0.5 else rng.randrange(i)
        edges.append((perm[i], perm[j]))
    return edges


if seed == 1:
    # single town, one airline, no flights
    print(1, 1, 0)
    sys.exit()

if seed == 2:
    n, m, k = 2000, 2000, 200000
    flights = []
    for a, b in spanning_tree(n, rng):
        flights.append((a, b, rng.randint(1, m), rng.randint(1, 100000)))
    while len(flights) < k:
        a, b = rng.sample(range(1, n + 1), 2)
        flights.append((a, b, rng.randint(1, m), rng.randint(1, 100000)))
    rng.shuffle(flights)
    emit(n, m, flights)
    sys.exit()

if seed == 3:
    # maximal, different structure: a handful of huge airlines, lots of
    # parallel flights between few town pairs, taxes drawn from a tiny set
    n, m, k = 2000, 2000, 200000
    big = rng.sample(range(1, m + 1), 5)
    taxes = [1, 2, 3, 99999, 100000]
    flights = []
    for a, b in spanning_tree(n, rng):
        c = rng.choice(big) if rng.random() < 0.9 else rng.randint(1, m)
        flights.append((a, b, c, rng.choice(taxes)))
    hubs = rng.sample(range(1, n + 1), 40)
    while len(flights) < k:
        if rng.random() < 0.7:
            a, b = rng.sample(hubs, 2)
        else:
            a, b = rng.sample(range(1, n + 1), 2)
        c = rng.choice(big) if rng.random() < 0.9 else rng.randint(1, m)
        flights.append((a, b, c, rng.choice(taxes)))
    rng.shuffle(flights)
    emit(n, m, flights)
    sys.exit()

n = rng.randint(2, 300)
m = rng.randint(1, 300)
flights = []

if seed == 4:
    # one airline already owns a full spanning tree -> optimal cost 0
    winner = rng.randint(1, m)
    for a, b in spanning_tree(n, rng):
        flights.append((a, b, winner, rng.randint(1, 100000)))
    for _ in range(rng.randint(0, 4 * n)):
        a, b = rng.sample(range(1, n + 1), 2)
        flights.append((a, b, rng.randint(1, m), rng.randint(1, 100000)))
elif seed == 5:
    # all taxes equal -> heavy ties between alternative optima
    for a, b in spanning_tree(n, rng):
        flights.append((a, b, rng.randint(1, m), 7))
    for _ in range(rng.randint(n, 6 * n)):
        a, b = rng.sample(range(1, n + 1), 2)
        flights.append((a, b, rng.randint(1, m), 7))
else:
    # random mid-size graphs, occasionally with parallel duplicates
    for a, b in spanning_tree(n, rng):
        flights.append((a, b, rng.randint(1, m), rng.randint(1, 100000)))
    for _ in range(rng.randint(0, 8 * n)):
        if flights and rng.random() < 0.3:
            a, b, _, _ = rng.choice(flights)
        else:
            a, b = rng.sample(range(1, n + 1), 2)
        flights.append((a, b, rng.randint(1, m), rng.randint(1, 100000)))

rng.shuffle(flights)
emit(n, m, flights)
