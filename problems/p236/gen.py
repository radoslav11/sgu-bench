"""Tests for p236 (Greedy Path): cycle maximizing cost/time ratio.

N <= 50 towns, at most one directed route per ordered pair, costs and
times in 1..100. Seed 2 is the complete digraph on 50 towns with random
weights; seed 3 keeps it complete but draws weights from a tiny set so
many cycles tie at the optimal ratio. Other seeds: acyclic graphs
(answer "0"), one long hidden cycle, 2-cycles with extreme ratios, and
random density sweeps.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    print(n, len(edges))
    for a, b, c, t in edges:
        print(a, b, c, t)


if seed == 1:
    emit(3, [(1, 2, 5, 7), (2, 3, 1, 1)])  # no cycle -> "0"
elif seed == 2:
    n = 50
    edges = [
        (a, b, rng.randint(1, 100), rng.randint(1, 100))
        for a in range(1, n + 1) for b in range(1, n + 1) if a != b
    ]
    emit(n, edges)
elif seed == 3:
    n = 50
    edges = [
        (a, b, rng.randint(1, 3), rng.randint(1, 3))
        for a in range(1, n + 1) for b in range(1, n + 1) if a != b
    ]
    emit(n, edges)
elif seed == 4:
    # complete DAG: only edges a -> b with a < b, no cycle at all
    n = 50
    edges = [
        (a, b, rng.randint(1, 100), rng.randint(1, 100))
        for a in range(1, n + 1) for b in range(a + 1, n + 1)
    ]
    emit(n, edges)
elif seed == 5:
    # one Hamiltonian cycle plus forward chords (still only one cycle base)
    n = 50
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    edges = []
    for i in range(n):
        edges.append(
            (perm[i], perm[(i + 1) % n], rng.randint(1, 100),
             rng.randint(1, 100))
        )
    for _ in range(300):
        i, j = sorted(rng.sample(range(n), 2))
        e = (perm[i], perm[j], rng.randint(1, 100), rng.randint(1, 100))
        if all((e[0], e[1]) != (x[0], x[1]) for x in edges):
            edges.append(e)
    emit(n, edges)
elif seed == 6:
    # disjoint 2-cycles with extreme ratios, best is 100/1 both ways
    n = 50
    edges = []
    for a in range(1, n, 2):
        b = a + 1
        if a == 17:
            edges += [(a, b, 100, 1), (b, a, 100, 1)]
        else:
            edges += [(a, b, rng.randint(1, 100), rng.randint(50, 100)),
                      (b, a, 1, 100)]
    emit(n, edges)
else:
    n = rng.randint(3, 50)
    pairs = [(a, b) for a in range(1, n + 1) for b in range(1, n + 1)
             if a != b]
    rng.shuffle(pairs)
    m = rng.randint(1, len(pairs))
    edges = [
        (a, b, rng.randint(1, 100), rng.randint(1, 100))
        for a, b in pairs[:m]
    ]
    emit(n, edges)
