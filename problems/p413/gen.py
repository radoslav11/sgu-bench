"""Tests for p413 (Berland Division): partition each connected even-n graph
into induced trees of at least two cities.

Each case is a connected simple graph on an even number of cities (no loops,
no parallel roads, a < b). A spanning tree is built over a random permutation
first to guarantee connectivity, then extra roads are added. The benchmark
caps total cities at 100 and total roads at 1000 across all cases of one file.

Seed 1 is the minimal single n=2 case. Seed 2 packs the limits into one
n=100, m=1000 dense case. Seed 3 splits the limits across 50 cases (mostly
n=2 edges plus one larger graph) to stress the per-case loop and force many
country relabelings. Later seeds use stars, paths, cycles, cliques and random
blends, which exercise the back-edge cutting in the reference's DP.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def connected_graph(n, m):
    # Spanning tree over a random permutation, then random extra simple edges.
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    edges = set()
    for i in range(1, n):
        a = perm[i]
        b = perm[rng.randrange(i)]
        edges.add((min(a, b), max(a, b)))
    max_edges = n * (n - 1) // 2
    m = min(m, max_edges)
    attempts = 0
    while len(edges) < m and attempts < m * 30 + 200:
        attempts += 1
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        if a != b:
            edges.add((min(a, b), max(a, b)))
    return sorted(edges)


def path_graph(n):
    return [(i, i + 1) for i in range(1, n)]


def star_graph(n):
    return [(1, i) for i in range(2, n + 1)]


def cycle_graph(n):
    e = [(i, i + 1) for i in range(1, n)]
    e.append((1, n))
    return sorted(e)


def clique(n):
    return [(a, b) for a in range(1, n + 1) for b in range(a + 1, n + 1)]


def emit(cases):
    out = [str(len(cases))]
    for n, edges in cases:
        out.append(f"{n} {len(edges)}")
        for a, b in edges:
            out.append(f"{a} {b}")
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit([(2, [(1, 2)])])
    sys.exit()

if seed == 2:
    # Single dense case using the full limits.
    emit([(100, connected_graph(100, 1000))])
    sys.exit()

if seed == 3:
    # Many cases: fill 50 cases. Total cities <= 100, roads <= 1000.
    cases = []
    cities_left = 100
    roads_left = 1000
    # one moderately sized graph first
    n0 = 50
    e0 = connected_graph(n0, 500)
    cases.append((n0, e0))
    cities_left -= n0
    roads_left -= len(e0)
    while len(cases) < 50 and cities_left >= 2:
        cases.append((2, [(1, 2)]))
        cities_left -= 2
        roads_left -= 1
    emit(cases)
    sys.exit()

if seed == 4:
    # Stars, paths, cycles, cliques mixed.
    cases = [
        (4, star_graph(4)),
        (6, path_graph(6)),
        (8, cycle_graph(8)),
        (6, clique(6)),
        (10, connected_graph(10, 15)),
    ]
    emit(cases)
    sys.exit()

if seed == 5:
    # A single large cycle plus chords (lots of back edges in any DFS tree).
    n = 100
    edges = set(cycle_graph(n))
    while len(edges) < 1000:
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        if a != b:
            edges.add((min(a, b), max(a, b)))
    emit([(n, sorted(edges))])
    sys.exit()

if seed == 6:
    # One big clique-ish graph (n=14 clique has 91 edges).
    n = 14
    emit([(n, clique(n))])
    sys.exit()

# Random blends of several cases within the global limits.
cases = []
cities_left = 100
roads_left = 1000
n_cases = rng.randint(1, 50)
for _ in range(n_cases):
    if cities_left < 2 or roads_left < 1:
        break
    max_n = min(cities_left, rng.choice([2, 4, 8, 20, 50, 100]))
    if max_n < 2:
        break
    n = rng.randrange(2, max_n + 1, 2) if max_n >= 2 else 2
    if n < 2:
        n = 2
    max_m = min(roads_left, n * (n - 1) // 2)
    m = rng.randint(n - 1, max(n - 1, max_m))
    edges = connected_graph(n, m)
    if len(edges) > roads_left:
        # trim to spanning tree if over budget
        edges = connected_graph(n, n - 1)
        if len(edges) > roads_left:
            break
    cases.append((n, edges))
    cities_left -= n
    roads_left -= len(edges)

if not cases:
    cases = [(2, [(1, 2)])]
emit(cases)
