"""Tests for p449 (Dendrograms): reorder leaves for a clean diagram.

N <= 100000 objects, M < N horizontal lines, Q <= min(1000, N) queries.
A merge sequence is built chronologically (bottom of the picture first)
with a DSU; each line gets a strictly smaller Y than the previous one,
so processing by decreasing Y reproduces the chronology. Each merged
group is named by a random member, and the lines are printed in a
shuffled order. Seed 1 is N=1, seed 2 a full binary-ish merge at max
size, seed 3 a path-shaped dendrogram of depth ~10^5. Later seeds mix
wide lines (large L), forests (M well below N-1) and small cases.
"""

import random
import sys


def gen_case(n, merges, rng, shuffle_lines=True):
    # merges: list of lists of group-indices to join, applied in order.
    # groups: list of (member list); we keep small random member samples.
    parent = list(range(n + 1))

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    reps = {i: i for i in range(1, n + 1)}  # root -> one member object
    lines = []
    y = 10**9 - rng.randint(0, 5)
    for grp in merges:
        members = []
        roots = []
        for g in grp:
            r = find(g)
            roots.append(r)
            members.append(reps[r])
        base = roots[0]
        for r in roots[1:]:
            parent[r] = base
        base = find(base)
        # representative of merged group: random object inside it
        reps[base] = rng.choice(members)
        rng.shuffle(members)
        lines.append((y, members))
        y -= rng.randint(1, 3)
        assert y >= 0

    if shuffle_lines:
        rng.shuffle(lines)

    q = min(1000, n)
    queries = rng.sample(range(1, n + 1), q)

    out = [f"{n} {len(lines)} {q}"]
    for yy, members in lines:
        out.append(f"{yy} {len(members)} " + ' '.join(map(str, members)))
    out.extend(map(str, queries))
    sys.stdout.write('\n'.join(out) + '\n')


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print("1 0 1")
    print("1")
elif seed == 2:
    # max size, random binary merges of random groups
    n = 100000
    alive = list(range(1, n + 1))
    merges = []
    while len(alive) > 1:
        i, j = rng.sample(range(len(alive)), 2)
        a, b = alive[i], alive[j]
        merges.append([a, b])
        alive.remove(b)
    gen_case(n, merges, rng)
elif seed == 3:
    # max size, caterpillar: one growing group absorbs leaves one by one
    n = 100000
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    merges = [[perm[0], perm[1]]]
    for v in perm[2:]:
        merges.append([perm[0], v])
    gen_case(n, merges, rng)
elif seed == 4:
    # max size, wide lines: few merges each joining many groups
    n = 100000
    alive = list(range(1, n + 1))
    merges = []
    while len(alive) > 1:
        l = min(len(alive), rng.randint(2, 300))
        chosen_idx = rng.sample(range(len(alive)), l)
        chosen = [alive[i] for i in chosen_idx]
        merges.append(chosen)
        keep = chosen[0]
        alive = [v for v in alive if v not in chosen[1:]]
    gen_case(n, merges, rng)
elif seed == 5:
    # forest: many components left unmerged
    n = 100000
    alive = list(range(1, n + 1))
    merges = []
    for _ in range(n // 3):
        i, j = rng.sample(range(len(alive)), 2)
        a, b = alive[i], alive[j]
        merges.append([a, b])
        alive.remove(b)
    gen_case(n, merges, rng)
elif seed == 6:
    # M = 0 (no lines at all)
    n = rng.randint(1000, 2000)
    gen_case(n, [], rng)
else:
    n = rng.randint(2, 60)
    alive = list(range(1, n + 1))
    merges = []
    target = rng.randint(0, len(alive) - 1)
    while len(alive) - 1 > n - 1 - target:
        l = min(len(alive), rng.randint(2, 4))
        chosen_idx = rng.sample(range(len(alive)), l)
        chosen = [alive[i] for i in chosen_idx]
        merges.append(chosen)
        alive = [v for v in alive if v not in chosen[1:]]
    gen_case(n, merges, rng)
