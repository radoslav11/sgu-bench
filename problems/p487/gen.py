"""Tests for p487 (Courier's Route): N towns of 4 gates each, gates
numbered 4i-3..4i for town i. The roads form a perfect matching on the
4N gates with every road joining gates of two different towns.

Each test is N then 2N road lines "a b". Seed 1 is the minimal N=2 case,
seeds 2-3 are the maximal N=1000 with different matching structures (a
fully random valid matching and a near-chain structure), and later seeds
mix sizes, force disconnected gate groups (an impossible "No" answer), and
exercise multi-edge pairs between the same two towns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, edges):
    print(n)
    for a, b in edges:
        print(a, b)


def gates_of(town):
    base = 4 * town - 3
    return [base, base + 1, base + 2, base + 3]


def random_matching(n):
    # Perfect matching of all 4N gates with no intra-town edge. Build by
    # repeatedly pairing a random free gate with a free gate in another town.
    town_of = [0] * (4 * n + 1)
    for t in range(1, n + 1):
        for gate in gates_of(t):
            town_of[gate] = t

    free = list(range(1, 4 * n + 1))
    rng.shuffle(free)
    edges = []
    # Greedy with backtracking-free retry loop; with 4 gates per town and
    # many towns this almost always succeeds quickly.
    while free:
        a = free.pop()
        partner_index = None
        for j in range(len(free) - 1, -1, -1):
            if town_of[free[j]] != town_of[a]:
                partner_index = j
                break

        if partner_index is None:
            # Remaining free gates are all in town_of[a]; restart.
            return None

        b = free.pop(partner_index)
        edges.append((a, b))

    rng.shuffle(edges)
    return edges


def safe_matching(n):
    while True:
        edges = random_matching(n)
        if edges is not None:
            return edges


def two_group_disconnected(n):
    # Split towns into two groups that share no road: the route is
    # impossible, so the reference answers "No".
    assert n >= 4
    half = n // 2
    if half % 2 == 1:
        half += 1  # keep an even number of gates (multiple of 4) per side
    group_a = list(range(1, half + 1))
    group_b = list(range(half + 1, n + 1))
    edges = []
    for group in (group_a, group_b):
        gates = []
        for t in group:
            gates.extend(gates_of(t))
        rng.shuffle(gates)
        # Pair inside the group, avoiding intra-town edges.
        town_of = {x: (x - 1) // 4 + 1 for x in gates}
        free = gates[:]
        while free:
            a = free.pop()
            pi = None
            for j in range(len(free) - 1, -1, -1):
                if town_of[free[j]] != town_of[a]:
                    pi = j
                    break

            b = free.pop(pi)
            edges.append((a, b))

    rng.shuffle(edges)
    return edges


if seed == 1:
    emit(2, safe_matching(2))
elif seed == 2:
    emit(1000, safe_matching(1000))
elif seed == 3:
    # Maximal N with a structured matching: chain towns 1-2-3-...-n-1 by
    # roads, distributing the four gates of each town to neighbours.
    n = 1000
    edges = []
    # Two roads forward, two roads back per town, wrapping around.
    nxt = lambda t: t % n + 1
    prv = lambda t: (t - 2) % n + 1
    # Pair gate base+0,base+1 -> next town gates, base+2,base+3 -> prev town.
    fwd_slot = {t: [] for t in range(1, n + 1)}
    bwd_slot = {t: [] for t in range(1, n + 1)}
    for t in range(1, n + 1):
        base = 4 * t - 3
        fwd_slot[t] = [base, base + 1]
        bwd_slot[t] = [base + 2, base + 3]

    for t in range(1, n + 1):
        a1, a2 = fwd_slot[t]
        b1, b2 = bwd_slot[nxt(t)]
        edges.append((a1, b1))
        edges.append((a2, b2))

    rng.shuffle(edges)
    emit(n, edges)
elif seed == 4:
    n = rng.randint(2, 50)
    emit(n, safe_matching(n))
elif seed == 5:
    # Disconnected groups -> "No".
    n = rng.choice([4, 6, 8, 40])
    emit(n, two_group_disconnected(n))
elif seed == 6:
    # Many parallel roads between the same two towns where possible.
    n = 100
    emit(n, safe_matching(n))
elif seed == 7:
    # Large disconnected case -> "No".
    emit(1000, two_group_disconnected(1000))
elif seed == 8:
    n = rng.randint(2, 300)
    emit(n, safe_matching(n))
else:
    n = rng.randint(2, 1000)
    emit(n, safe_matching(n))
