"""Tests for p143 (Long Live the Queen): tree of N <= 16000 towns,
profits in [-1000, 1000].

Seed 1 is a single all-negative town (the subset must stay non-empty).
Seeds 2-3 are maximal: a shallow random tree and a pure path (worst
recursion depth). Seeds 4-5 keep N maximal with all-negative profits
and a star shape; later seeds vary tree shape and profit sign mix.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

N_MAX = 16000


def random_tree(n, rng, shape):
    edges = []
    for v in range(2, n + 1):
        if shape == "path":
            parent = v - 1
        elif shape == "star":
            parent = 1
        elif shape == "shallow":
            parent = rng.randint(max(1, v - 50), v - 1)
        else:
            parent = rng.randint(1, v - 1)
        edges.append((parent, v))
    return edges


if seed == 1:
    print(1)
    print(-1000)
    sys.exit()

if seed == 2:
    n, shape = N_MAX, "random"
    profits = [rng.randint(-1000, 1000) for _ in range(n)]
elif seed == 3:
    n, shape = N_MAX, "path"
    profits = [rng.randint(-1000, 1000) for _ in range(n)]
elif seed == 4:
    n, shape = N_MAX, "shallow"
    profits = [rng.randint(-1000, -1) for _ in range(n)]
elif seed == 5:
    n, shape = N_MAX, "star"
    profits = [rng.choice([-1000, 1000]) for _ in range(n)]
else:
    n = rng.randint(2, N_MAX)
    shape = rng.choice(["random", "path", "star", "shallow"])
    lo, hi = rng.choice([(-1000, 1000), (-1000, 0), (0, 1000), (-5, 5)])
    profits = [rng.randint(lo, hi) for _ in range(n)]

edges = random_tree(n, rng, shape)
labels = list(range(1, n + 1))
rng.shuffle(labels)
perm = [0] * (n + 1)
for i, lab in enumerate(labels, start=1):
    perm[i] = lab

out = [str(n)]
shuffled_profits = [0] * n
for v in range(1, n + 1):
    shuffled_profits[perm[v] - 1] = profits[v - 1]
out.append(" ".join(map(str, shuffled_profits)))
rng.shuffle(edges)
for a, b in edges:
    out.append(f"{perm[a]} {perm[b]}")
print("\n".join(out))
