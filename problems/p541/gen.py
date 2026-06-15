"""Tests for p541 (BR Privatization): a ring of k >= 3 areas, each a cycle
of >= 3 stations, adjacent areas sharing one degree-4 junction station.

The graph is built directly from its structure: k junctions placed on a
meta-cycle, and for each area two interior chains of fresh degree-2 nodes
linking its two junctions. n = sum(sizes) - k, m = sum(sizes), with
6 <= n <= 1e5 and 9 <= m <= 1e5.

Seed 1 is the minimal legal network (3 triangle-areas). Seeds 2 and 3 are
maximal: seed 2 maximizes the number of areas (many tiny areas), seed 3
uses few large areas; both push m close to 1e5. Later seeds randomize the
area count and sizes, mixing odd and even areas and a final relabeling so
station ids are not in structural order.
"""

import random
import sys

MAXN = 10 ** 5
MAXM = 10 ** 5


def build(k, sizes, rng):
    # sizes[i] = number of stations in area i (cycle length), >= 3.
    # junction[i] is shared between area i-1 and area i.
    next_id = 1
    junction = []
    for _ in range(k):
        junction.append(next_id)
        next_id += 1

    edges = []
    for i in range(k):
        ja = junction[i]
        jb = junction[(i + 1) % k]
        interior_total = sizes[i] - 2  # >= 1
        # Split interior vertices into the two arcs, each arc may be empty
        # but not both (size >= 3 guarantees interior_total >= 1).
        la = rng.randint(0, interior_total)
        # Build arc A: ja -> a1 -> ... -> a_la -> jb
        prev = ja
        for _ in range(la):
            v = next_id
            next_id += 1
            edges.append((prev, v))
            prev = v
        edges.append((prev, jb))
        # Build arc B: ja -> b1 -> ... -> jb (uses remaining interior nodes)
        lb = interior_total - la
        prev = ja
        for _ in range(lb):
            v = next_id
            next_id += 1
            edges.append((prev, v))
            prev = v
        edges.append((prev, jb))

    n = next_id - 1
    return n, edges


def emit(n, edges, rng, relabel=True):
    m = len(edges)
    if relabel:
        perm = list(range(1, n + 1))
        rng.shuffle(perm)
        mp = {i + 1: perm[i] for i in range(n)}
        edges = [(mp[a], mp[b]) for a, b in edges]
    rng.shuffle(edges)
    out = [f"{n} {m}"]
    for a, b in edges:
        if rng.random() < 0.5:
            a, b = b, a
        out.append(f"{a} {b}")
    sys.stdout.write("\n".join(out) + "\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    k = 3
    sizes = [3, 3, 3]
    n, edges = build(k, sizes, rng)
    emit(n, edges, rng)
    sys.exit()

if seed == 2:
    # Maximize area count: each area size 3 (one interior node). Then
    # m = 3k, n = 2k. Cap by m <= 1e5 => k <= 33333.
    k = 33333
    sizes = [3] * k
    n, edges = build(k, sizes, rng)
    emit(n, edges, rng)
    sys.exit()

if seed == 3:
    # Few large areas; push m toward 1e5. m = sum sizes.
    k = 3
    base = MAXM // 3
    sizes = [base, base, MAXM - 2 * base]
    n, edges = build(k, sizes, rng)
    # n = m - k; ensure n <= MAXN.
    while (sum(sizes) - k) > MAXN or sum(sizes) > MAXM:
        sizes[0] -= 1
    n, edges = build(k, sizes, rng)
    emit(n, edges, rng)
    sys.exit()

# Randomized mid-size networks.
k = rng.randint(3, 200)
# Choose sizes with a target total edge count.
target_m = rng.randint(9, min(MAXM, MAXN + k))
# Start each area at minimum size 3, then distribute extra.
sizes = [3] * k
extra = target_m - 3 * k
if extra < 0:
    extra = 0
for _ in range(extra):
    sizes[rng.randrange(k)] += 1
# Respect n <= MAXN and m <= MAXM.
while sum(sizes) > MAXM or (sum(sizes) - k) > MAXN:
    j = max(range(k), key=lambda t: sizes[t])
    if sizes[j] <= 3:
        break
    sizes[j] -= 1

n, edges = build(k, sizes, rng)
emit(n, edges, rng)
