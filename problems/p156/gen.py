"""Tests for p156 (Strange Graph): N <= 10000, M <= 100000.

A strange graph is built as hub-cliques (size >= 3, every hub gets
exactly one chain stub) joined by chains of degree-2 vertices; a chain
between two hubs of the same clique gets at least 2 internal vertices
so the deg-2 condition (non-adjacent neighbours) holds. A Hamiltonian
cycle exists iff every clique has even size, so odd cliques give -1
cases. Seed 1 is the minimal C4, seed 2 one huge clique with M near
100000, seed 3 a max-N web of 4-cliques, seed 4 a max-N -1 case with
odd cliques, seed 5 a pure 10000-cycle; the rest are random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges, relabel=True):
    perm = list(range(n))
    if relabel:
        rng.shuffle(perm)
        rng.shuffle(edges)
    lines = [f"{n} {len(edges)}"]
    for u, v in edges:
        a, b = perm[u] + 1, perm[v] + 1
        if rng.random() < 0.5:
            a, b = b, a
        lines.append(f"{a} {b}")
    sys.stdout.write("\n".join(lines) + "\n")


def build(sizes, total_n):
    """Cliques of the given sizes plus chains using up total_n vertices."""
    vid = 0
    edges = []
    cliques = []
    for c in sizes:
        members = list(range(vid, vid + c))
        vid += c
        cliques.append(members[:])
        for i in range(c):
            for j in range(i + 1, c):
                edges.append((members[i], members[j]))

    free = [mem[:] for mem in cliques]
    chains = []  # (u, v, same_clique)
    b = len(cliques)
    if b > 1:
        for k in range(b):
            u = free[k].pop()
            v = free[(k + 1) % b].pop(0)
            chains.append((u, v, False))
    rest = []
    for k, mem in enumerate(free):
        rest.extend((h, k) for h in mem)
    rng.shuffle(rest)
    for i in range(0, len(rest), 2):
        (u, ku), (v, kv) = rest[i], rest[i + 1]
        chains.append((u, v, ku == kv))

    budget = total_n - vid
    lens = []
    for _, _, same in chains:
        lens.append(2 if same else 1)
    budget -= sum(lens)
    while budget > 0:
        i = rng.randrange(len(lens))
        add = min(budget, rng.randint(1, 40))
        lens[i] += add
        budget -= add

    for (u, v, _), k in zip(chains, lens):
        prev = u
        for _ in range(k):
            edges.append((prev, vid))
            prev = vid
            vid += 1
        edges.append((prev, v))
    return vid, edges


if seed == 1:
    emit(4, [(0, 1), (1, 2), (2, 3), (3, 0)], relabel=False)
elif seed == 2:
    n, edges = build([420], 10000)
    emit(n, edges)
elif seed == 3:
    n, edges = build([4] * 600, 10000)
    emit(n, edges)
elif seed == 4:
    sizes = [4] * 598 + [3, 5]
    rng.shuffle(sizes)
    n, edges = build(sizes, 10000)
    emit(n, edges)
elif seed == 5:
    n = 10000
    emit(n, [(i, (i + 1) % n) for i in range(n)])
else:
    b = rng.randint(1, 25)
    sizes = [rng.randint(3, 12) for _ in range(b)]
    if seed % 2 == 0:
        sizes = [s + (s % 2) for s in sizes]  # all even -> cycle exists
    if sum(sizes) % 2 == 1:
        sizes[0] += 1
    hubs = sum(sizes)
    n_total = hubs + rng.randint(hubs, hubs * 4) + 2 * len(sizes)
    n, edges = build(sizes, n_total)
    emit(n, edges)
