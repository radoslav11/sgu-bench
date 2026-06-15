"""Tests for p164 (Airlines): buy <= (M+1)/2 parts so free flights have
diameter <= 3.

The flight network is the complete graph on N <= 200 cities; every edge
is owned by exactly one of M parts and every part owns at least one
edge. The generator assigns parts to the C(N,2) edges, always making
the assignment surjective by giving the first M shuffled edges parts
1..M.

Seed 1 is minimal (2 cities, 1 part), seed 2 maximal with every edge
its own part (M = 19900), seed 3 maximal with M = 2 and an adversarial
split: odd part inside two cliques only, so the odd graph is
disconnected and even must be chosen. Later seeds mix star ownership,
random M, and the statement sample.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, owner):
    print(n, m)
    out = []
    for i in range(n):
        out.append(" ".join(str(owner[i][j]) for j in range(n)))
    print("\n".join(out))


def assign(n, m, pick, rng):
    # pick(i, j) -> part id or None for "random"; the first m shuffled
    # unassigned-by-pick edges then get parts 1..m so all parts exist
    owner = [[0] * n for _ in range(n)]
    edges = [(i, j) for i in range(n) for j in range(i + 1, n)]
    rng.shuffle(edges)
    forced = []
    free = []
    for i, j in edges:
        p = pick(i, j)
        if p is None:
            free.append((i, j))
        else:
            forced.append((i, j, p))
    used = set(p for _, _, p in forced)
    missing = [p for p in range(1, m + 1) if p not in used]
    for idx, (i, j) in enumerate(free):
        if idx < len(missing):
            p = missing[idx]
        else:
            p = rng.randint(1, m)
        owner[i][j] = owner[j][i] = p
    for i, j, p in forced:
        owner[i][j] = owner[j][i] = p
    return owner


if seed == 1:
    emit(2, 1, [[0, 1], [1, 0]])
elif seed == 2:
    # maximal: every edge is its own part
    n = 200
    m = n * (n - 1) // 2
    parts = list(range(1, m + 1))
    rng.shuffle(parts)
    owner = [[0] * n for _ in range(n)]
    idx = 0
    for i in range(n):
        for j in range(i + 1, n):
            owner[i][j] = owner[j][i] = parts[idx]
            idx += 1
    emit(n, m, owner)
elif seed == 3:
    # maximal, M = 2: part 1 only inside two halves (disconnected),
    # part 2 on all cross edges (complete bipartite, diameter 2)
    n = 200
    half = set(rng.sample(range(n), n // 2))
    owner = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            same = (i in half) == (j in half)
            owner[i][j] = owner[j][i] = 1 if same else 2
    emit(n, 2, owner)
elif seed == 4:
    # statement sample
    print("4 3")
    print("0 3 2 2")
    print("3 0 1 2")
    print("2 1 0 1")
    print("2 2 1 0")
elif seed == 5:
    # part 1 owns a star at city 0, the rest random among 2..m
    n = 200
    m = 100

    def pick(i, j):
        return 1 if i == 0 or j == 0 else None

    emit(n, m, assign(n, m, pick, rng))
elif seed == 6:
    # odd parts live on a hamiltonian cycle, even parts elsewhere
    n = 200
    m = 400
    cycle = list(range(n))
    rng.shuffle(cycle)
    on_cycle = set()
    for k in range(n):
        a, b = cycle[k], cycle[(k + 1) % n]
        on_cycle.add((min(a, b), max(a, b)))

    def pick(i, j):
        if (i, j) in on_cycle:
            return rng.randrange(1, m + 1, 2)
        return rng.randrange(2, m + 1, 2)

    emit(n, m, assign(n, m, pick, rng))
else:
    n = rng.randint(2, 200)
    max_m = n * (n - 1) // 2
    m = rng.randint(1, min(max_m, rng.choice([3, 17, 500, max_m])))
    emit(n, m, assign(n, m, lambda i, j: None, rng))
