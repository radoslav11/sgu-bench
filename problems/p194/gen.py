"""Tests for p194 (Reactor Cooling): N <= 200 nodes, at most one pipe
per unordered node pair (so M <= 19900), 0 <= l <= c <= 10^5.

Feasible instances are built by superposing random directed cycles on
the complete graph: the signed per-pair sums form a circulation f, each
pipe gets direction sign(f), l <= f and c >= f. Seed 1 is N=1, M=0.
Seed 2 is N=200 with all 19900 pipes, feasible with slack. Seed 3 is the
same size but fully tight (l = c = f), still feasible. Seed 4 is maximal
and infeasible: around node 1 the outgoing lower bounds exceed the
incoming capacities (violated cut S = {1}). Seeds 5+ are mid-size mixed
feasible / cut-broken / random instances.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
MAXF = 10**5


def feasible_instance(n, cycles, amount_hi, tight, slack_hi):
    flow = {}  # (u, v) with u < v -> signed flow (positive means u -> v)
    for _ in range(cycles):
        k = rng.randint(3, min(n, 12))
        nodes = rng.sample(range(1, n + 1), k)
        amt = rng.randint(0, amount_hi)
        for a, b in zip(nodes, nodes[1:] + nodes[:1]):
            if a < b:
                flow[(a, b)] = flow.get((a, b), 0) + amt
            else:
                flow[(b, a)] = flow.get((b, a), 0) - amt
    edges = []
    for u in range(1, n + 1):
        for v in range(u + 1, n + 1):
            f = flow.get((u, v), 0)
            a, b, f = (u, v, f) if f >= 0 else (v, u, -f)
            assert f <= MAXF
            if tight:
                lo = hi = f
            else:
                lo = max(0, f - rng.randint(0, slack_hi))
                hi = min(MAXF, f + rng.randint(0, slack_hi))
            edges.append((a, b, lo, hi))
    return edges


def break_cut(edges, x):
    """Make the cut S = {x} violated: pipes out of x demand far more in
    total than the pipes into x can carry."""
    out_deg = sum(1 for a, b, lo, hi in edges if a == x)
    in_deg = sum(1 for a, b, lo, hi in edges if b == x)
    if out_deg == 0:
        edges = [
            ((b, a, 0, hi) if b == x else (a, b, lo, hi))
            for a, b, lo, hi in edges
        ]
        out_deg, in_deg = in_deg, 0
    demand = in_deg + 1  # per out-pipe lower bound beats total in-capacity
    fixed = []
    for a, b, lo, hi in edges:
        if a == x:
            fixed.append((a, b, demand, max(demand, hi)))
        elif b == x:
            fixed.append((a, b, 0, rng.randint(0, 1)))
        else:
            fixed.append((a, b, lo, hi))
    return fixed


def emit(n, edges):
    edges = edges[:]
    rng.shuffle(edges)
    print(n, len(edges))
    for a, b, lo, hi in edges:
        print(a, b, lo, hi)


if seed == 1:
    print(1, 0)
elif seed == 2:
    emit(200, feasible_instance(200, 4000, 40, False, 30))
elif seed == 3:
    emit(200, feasible_instance(200, 4000, 40, True, 0))
elif seed == 4:
    edges = break_cut(feasible_instance(200, 4000, 40, False, 30), 1)
    emit(200, edges)
else:
    n = rng.randint(2, 60)
    edges = feasible_instance(
        n, rng.randint(1, 200), 60, rng.random() < 0.3, rng.randint(0, 20)
    )
    keep = rng.random()
    edges = [e for e in edges if e[3] > 0 or rng.random() < keep]
    if rng.random() < 0.5 and edges:
        a0, b0, _, _ = edges[rng.randrange(len(edges))]
        edges = break_cut(edges, rng.choice([a0, b0]))
    if not edges:
        edges = [(1, 2, 0, 5)]
    emit(n, edges)
