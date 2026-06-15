"""Tests for p525 (Revolutionary Roads): n<=1000 cities, m<=20000 one-way roads.

No self loops; at most one road per ordered pair (both directions allowed).
Seed 1 is the minimal n=1,m=0 case. Seeds 2-3 are maximal m=20000: seed 2 is
a long chain of small SCCs (so a single flip can merge a long path), seed 3 is
a random dense digraph. Later seeds cover the no-edge case, a single big cycle,
many parallel two-cycles, and random mid-size graphs.
"""

import random
import sys

N_MAX = 1000
M_MAX = 20000


def emit(n, edges):
    print(n, len(edges))
    out = []
    for u, v in edges:
        out.append(f"{u} {v}")
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, [])
    sys.exit()

if seed == 2:
    # Chain of 2-cycles linked forward: components c0->c1->...; flipping a
    # forward link can merge a long prefix/suffix into one big SCC.
    n = 1000
    used = set()
    edges = []
    # pair up nodes into 2-cycles: (2k+1, 2k+2)
    k = n // 2
    for i in range(k):
        a = 2 * i + 1
        b = 2 * i + 2
        edges.append((a, b)); used.add((a, b))
        edges.append((b, a)); used.add((b, a))
    # forward links between consecutive pairs
    for i in range(k - 1):
        a = 2 * i + 2
        b = 2 * (i + 1) + 1
        if (a, b) not in used:
            edges.append((a, b)); used.add((a, b))
    # fill remaining capacity with random forward edges (keep it a DAG-ish
    # structure among pairs to avoid one giant SCC already)
    while len(edges) < M_MAX:
        i = rng.randint(0, k - 1)
        j = rng.randint(0, k - 1)
        if i >= j:
            continue
        a = rng.choice([2 * i + 1, 2 * i + 2])
        b = rng.choice([2 * j + 1, 2 * j + 2])
        if a != b and (a, b) not in used:
            used.add((a, b))
            edges.append((a, b))
    emit(n, edges[:M_MAX])
    sys.exit()

if seed == 3:
    # Maximal random dense digraph.
    n = 1000
    used = set()
    edges = []
    while len(edges) < M_MAX:
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        if a != b and (a, b) not in used:
            used.add((a, b))
            edges.append((a, b))
    emit(n, edges)
    sys.exit()

if seed == 4:
    # No edges, larger n.
    emit(rng.randint(1, N_MAX), [])
    sys.exit()

if seed == 5:
    # One big Hamiltonian cycle missing one closing edge: flipping the right
    # edge could close it, but here it is a path of 2-cycles to be merged.
    n = 1000
    used = set()
    edges = []
    for i in range(1, n):
        edges.append((i, i + 1)); used.add((i, i + 1))
    # add a back edge that, when flipped, makes a long SCC
    edges.append((n, 1)); used.add((n, 1))
    # pad with random
    while len(edges) < M_MAX:
        a = rng.randint(1, n)
        b = rng.randint(1, n)
        if a != b and (a, b) not in used:
            used.add((a, b))
            edges.append((a, b))
    emit(n, edges[:M_MAX])
    sys.exit()

if seed == 6:
    # Many independent 2-cycles (lots of equal-size SCCs, lots of ties).
    n = N_MAX
    used = set()
    edges = []
    k = n // 2
    for i in range(k):
        a = 2 * i + 1
        b = 2 * i + 2
        edges.append((a, b)); used.add((a, b))
        edges.append((b, a)); used.add((b, a))
    emit(n, edges)
    sys.exit()

# Random mid-size graphs.
n = rng.randint(2, 600)
m_target = rng.randint(0, min(M_MAX, n * (n - 1)))
used = set()
edges = []
attempts = 0
while len(edges) < m_target and attempts < m_target * 5 + 100:
    attempts += 1
    a = rng.randint(1, n)
    b = rng.randint(1, n)
    if a != b and (a, b) not in used:
        used.add((a, b))
        edges.append((a, b))
emit(n, edges)
