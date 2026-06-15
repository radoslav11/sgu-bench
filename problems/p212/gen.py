"""Tests for p212 (Data Transmission): layered network, N <= 1500,
M <= 300000, levels 1..L with unique source (level 1) and target
(level L); every channel goes from level i to level i+1, pairs unique.

Seed 1 is minimal (N=2, M=1). Seed 2 is maximal-M: two huge middle
layers with 300000 channels. Seed 3 is maximal too but deep (many wide
layers, mixed capacities). Seed 4 is the longest chain (L=N=1500).
Seeds 5+ are random layered networks with assorted widths and caps.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXM = 300000


def emit(levels, edges):
    # levels: list over nodes 1..n; edges: (a, b, c)
    n = len(levels)
    perm = list(range(1, n + 1))
    rng.shuffle(perm)  # relabel so the source is not always node 1
    new_levels = [0] * n
    for old, new in zip(range(1, n + 1), perm):
        new_levels[new - 1] = levels[old - 1]
    print(n, len(edges), max(levels))
    print(" ".join(map(str, new_levels)))
    out = []
    for a, b, c in edges:
        out.append(f"{perm[a - 1]} {perm[b - 1]} {c}")
    print("\n".join(out))


def layered(widths, m_target, cap_lo, cap_hi):
    # widths[0] == 1 (source layer) and widths[-1] == 1 (target layer)
    levels = []
    layer_nodes = []
    nxt = 1
    for w in widths:
        layer_nodes.append(list(range(nxt, nxt + w)))
        levels.extend([len(layer_nodes)] * w)
        nxt += w
    edges = []
    # guarantee connectivity layer to layer first
    for la, lb in zip(layer_nodes, layer_nodes[1:]):
        for b in lb:
            a = rng.choice(la)
            edges.append((a, b, rng.randint(cap_lo, cap_hi)))
    used = set((a, b) for a, b, _ in edges)
    pairs_per_gap = [
        (i, len(layer_nodes[i]) * len(layer_nodes[i + 1]))
        for i in range(len(layer_nodes) - 1)
    ]
    attempts = 0
    while len(edges) < m_target and attempts < 40 * m_target:
        attempts += 1
        i = rng.randrange(len(layer_nodes) - 1)
        a = rng.choice(layer_nodes[i])
        b = rng.choice(layer_nodes[i + 1])
        if (a, b) in used:
            continue
        used.add((a, b))
        edges.append((a, b, rng.randint(cap_lo, cap_hi)))
    rng.shuffle(edges)
    return levels, edges


if seed == 1:
    print(2, 1, 2)
    print(1, 2)
    print(1, 2, 1)
    sys.exit()

if seed == 2:
    # two middle layers of 749 nodes each: room for far more than 300k
    widths = [1, 749, 749, 1]
    levels, edges = layered(widths, MAXM, 1, 10**6)
elif seed == 3:
    # deep and wide: 8 middle layers of ~187 nodes
    widths = [1] + [187] * 8 + [1]
    levels, edges = layered(widths, MAXM, 1, 100)
elif seed == 4:
    # longest chain: L = N = 1500
    n = 1500
    levels = list(range(1, n + 1))
    edges = [(i, i + 1, rng.randint(1, 10**6)) for i in range(1, n)]
elif seed == 5:
    # all capacities equal: heavy ties
    widths = [1, 400, 400, 400, 1]
    levels, edges = layered(widths, 200000, 7, 7)
else:
    l_mid = rng.randint(1, 12)
    widths = [1] + [rng.randint(1, 150) for _ in range(l_mid)] + [1]
    n = sum(widths)
    m_target = rng.randint(n, min(MAXM, 4 * n * n))
    levels, edges = layered(widths, m_target, 1, rng.choice([1, 50, 10**6]))

emit(levels, edges)
