"""Tests for p103 (Traffic Lights): N <= 300 junctions, M <= 14000 roads.

Junction line is "C r tB tP" with 1 <= r <= duration of the initial
color C. Seed 1 is the 2-junction minimum, seeds 2-3 are maximal dense
graphs (random lights vs. adversarial synchronized opposite-phase
lights), seed 4 is disconnected (no path), seed 5 blocks the only road
to the destination with lights that never show equal colors.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_light():
    tB, tP = rng.randint(1, 100), rng.randint(1, 100)
    if rng.random() < 0.5:
        return ("B", rng.randint(1, tB), tB, tP)
    return ("P", rng.randint(1, tP), tB, tP)


def emit(src, dst, n, lights, edges):
    print(src, dst)
    print(n, len(edges))
    for c, r, tb, tp in lights:
        print(c, r, tb, tp)
    for u, v, w in edges:
        print(u, v, w)


def random_edges(nodes, m):
    pairs = [(a, b) for i, a in enumerate(nodes) for b in nodes[i + 1:]]
    chosen = rng.sample(pairs, min(m, len(pairs)))
    return [(u, v, rng.randint(1, 100)) for u, v in chosen]


if seed == 1:
    emit(1, 2, 2, [rand_light(), rand_light()], [(1, 2, rng.randint(1, 100))])
elif seed in (2, 3):
    n = 300
    lights = []
    for i in range(n):
        if seed == 3 and i % 2 == 0:
            # Period-100 lights, half of them phase-flipped: many road
            # endpoints switch simultaneously and rarely match.
            lights.append(("B" if i % 4 == 0 else "P", 50, 50, 50))
        else:
            lights.append(rand_light())
    edges = random_edges(list(range(1, n + 1)), 14000)
    emit(1, n, n, lights, edges)
elif seed == 4:
    # Destination 300 is isolated: answer 0.
    n = 300
    edges = random_edges(list(range(1, 151)), 5000)
    emit(1, n, n, [rand_light() for _ in range(n)], edges)
elif seed == 5:
    # Node n reachable only through n-1, but the two lights switch at
    # the same instants with opposite colors forever: answer 0.
    n = 50
    lights = [rand_light() for _ in range(n - 2)]
    lights.append(("B", 50, 50, 50))
    lights.append(("P", 50, 50, 50))
    edges = random_edges(list(range(1, n)), 300)
    edges.append((n - 1, n, rng.randint(1, 100)))
    emit(1, n, n, lights, edges)
else:
    n = rng.randint(2, 120)
    max_m = n * (n - 1) // 2
    edges = random_edges(list(range(1, n + 1)), rng.randint(1, min(max_m, 2000)))
    src = rng.randint(1, n)
    dst = rng.randint(1, n)
    while dst == src:
        dst = rng.randint(1, n)
    emit(src, dst, n, [rand_light() for _ in range(n)], edges)
