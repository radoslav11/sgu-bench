"""Tests for p513 (Maximal Clique reduction recognition).

1 <= v <= 100 vertices, edges given once, no self-loops. Output is YES iff the
graph is the 3-SAT-to-clique reduction graph (with vertices arbitrarily
permuted). A reduction graph has 3c vertices for c clauses: the 3 vertices of
a clause form an independent set, and across clauses every pair is connected
except contradictory literal pairs (same variable, opposite sign).

YES instances are produced by actually performing the reduction on a random
3-SAT formula and randomly relabelling the vertices. Seed 1 is the minimal
YES case (a single clause, v=3). Seed 2 is the maximal YES case (33 clauses,
v=99, densest reduction). Seed 3 is the maximal NO case (v=100, a dense
near-reduction). The remaining seeds mix random NO graphs, near-miss graphs
obtained by perturbing a valid reduction, and v not divisible by 3.
"""

import random
import sys

VMAX = 100

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edge_set):
    edges = sorted(edge_set)
    out = [f"{n} {len(edges)}"]
    out += [f"{u} {v}" for u, v in edges]
    sys.stdout.write("\n".join(out) + "\n")


def reduction_edges(clauses, rng):
    # clauses: list of [(var, sign), (var, sign), (var, sign)]
    verts = []
    for ci, cl in enumerate(clauses):
        for (var, sign) in cl:
            verts.append((ci, var, sign))
    n = len(verts)
    label = list(range(n))
    rng.shuffle(label)
    es = set()
    for a in range(n):
        ia, va, sa = verts[a]
        for b in range(a + 1, n):
            ib, vb, sb = verts[b]
            if ia != ib and not (va == vb and sa != sb):
                u, w = label[a] + 1, label[b] + 1
                es.add((min(u, w), max(u, w)))
    return n, es


def random_formula(c, nvars, rng):
    clauses = []
    for _ in range(c):
        vs = rng.sample(range(nvars), 3)
        clauses.append([(v, rng.randint(0, 1)) for v in vs])
    return clauses


def random_graph(n, m, rng):
    all_pairs = [(i, j) for i in range(1, n + 1) for j in range(i + 1, n + 1)]
    m = min(m, len(all_pairs))
    chosen = rng.sample(all_pairs, m)
    return set(chosen)


if seed == 1:
    # minimal YES: a single clause -> 3 independent vertices
    n, es = reduction_edges(random_formula(1, 3, rng), rng)
    emit(n, es)
    sys.exit()

if seed == 2:
    # maximal YES: 33 clauses, v = 99, many variables so few contradictions
    n, es = reduction_edges(random_formula(33, 40, rng), rng)
    emit(n, es)
    sys.exit()

if seed == 3:
    # maximal NO: v = 100 dense near-reduction (start from a 33-clause
    # reduction on 99 vertices, then add a 100th vertex wired densely)
    n, es = reduction_edges(random_formula(33, 35, rng), rng)
    n = 100
    for v in range(1, 100):
        if rng.random() < 0.7:
            es.add((min(v, 100), max(v, 100)))
    emit(n, es)
    sys.exit()

if seed == 4:
    # YES with many contradictions: few variables -> dense non-edge structure
    n, es = reduction_edges(random_formula(33, 4, rng), rng)
    emit(n, es)
    sys.exit()

if seed == 5:
    # near-miss NO: valid reduction with one edge toggled
    n, es = reduction_edges(random_formula(rng.randint(5, 33), 12, rng), rng)
    es = set(es)
    # toggle a random pair
    a = rng.randint(1, n)
    b = rng.randint(1, n)
    while b == a:
        b = rng.randint(1, n)
    pair = (min(a, b), max(a, b))
    if pair in es:
        es.discard(pair)
    else:
        es.add(pair)
    emit(n, es)
    sys.exit()

if seed == 6:
    # v not divisible by 3 -> always NO
    v = rng.choice([1, 2, 4, 5, 7, 100, 98])
    maxm = v * (v - 1) // 2
    m = rng.randint(0, maxm)
    emit(v, random_graph(v, m, rng))
    sys.exit()

if seed == 7:
    # dense random NO graph on v=99 (divisible by 3, unlikely to be a reduction)
    v = 99
    maxm = v * (v - 1) // 2
    m = rng.randint(maxm // 2, maxm)
    emit(v, random_graph(v, m, rng))
    sys.exit()

# seeds 8+: mixed random. Half attempt a valid reduction, half a random graph.
if rng.random() < 0.5:
    c = rng.randint(1, 33)
    nvars = rng.randint(3, 30)
    n, es = reduction_edges(random_formula(c, nvars, rng), rng)
    emit(n, es)
else:
    v = rng.randint(1, 100)
    maxm = v * (v - 1) // 2
    m = rng.randint(0, maxm)
    emit(v, random_graph(v, m, rng))
