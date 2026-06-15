"""Tests for p321 (The Spy Network): tree of N <= 200000 spies rooted at
Kat (node 1), each channel line is "child parent almost protected" or
"child parent protected", channels numbered by input order.

Depth is kept <= 50000 because deeper recursion is not survivable with
the default 8 MB stack in this environment. Seed 1 is N=1, seed 2 four
50000-deep all-almost chains (max N, max depth, ~100k conversions),
seed 3 a maximal random shallow tree, later seeds mix shapes, type
ratios and label permutations.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXD = 50000


def emit(parent, types, n):
    # parent[i], types[i] for nodes 2..n; channel i-1 is line order below
    lines = [str(n)]
    for i in range(2, n + 1):
        t = "almost protected" if types[i] else "protected"
        lines.append(f"{i} {parent[i]} {t}")
    sys.stdout.write("\n".join(lines) + "\n")


if seed == 1:
    print(1)
    sys.exit()

if seed == 2:
    n = 200000
    parent = [0] * (n + 1)
    types = [1] * (n + 1)
    node = 2
    for _ in range(4):
        prev = 1
        for _ in range(MAXD):
            if node > n:
                break
            parent[node] = prev
            prev = node
            node += 1
    emit(parent, types, n)
    sys.exit()

if seed == 3:
    n = 200000
    parent = [0] * (n + 1)
    types = [0] * (n + 1)
    for i in range(2, n + 1):
        parent[i] = rng.randint(1, i - 1)
        types[i] = rng.random() < 0.7
    emit(parent, types, n)
    sys.exit()

if seed == 4:
    # max-size caterpillar: 50000 spine, hairs attached, random types
    n = 200000
    parent = [0] * (n + 1)
    types = [0] * (n + 1)
    spine = [1]
    for i in range(2, MAXD + 2):
        parent[i] = i - 1
        spine.append(i)
    for i in range(MAXD + 2, n + 1):
        parent[i] = rng.choice(spine)
    for i in range(2, n + 1):
        types[i] = rng.random() < 0.5
    emit(parent, types, n)
    sys.exit()

n = rng.randint(2, 3000)
parent = [0] * (n + 1)
types = [0] * (n + 1)
p_almost = rng.choice([0.1, 0.5, 0.9, 1.0])
shape = seed % 3
for i in range(2, n + 1):
    if shape == 0:
        parent[i] = rng.randint(1, i - 1)
    elif shape == 1:
        parent[i] = max(1, i - rng.randint(1, 4))
    else:
        parent[i] = max(1, i - 1 if rng.random() < 0.8 else rng.randint(1, i - 1))
    types[i] = rng.random() < p_almost

# relabel nodes randomly, keeping Kat as node 1
perm = list(range(2, n + 1))
rng.shuffle(perm)
label = [0, 1] + perm
new_parent = [0] * (n + 1)
new_types = [0] * (n + 1)
order = []
for i in range(2, n + 1):
    order.append((label[i], label[parent[i]], types[i]))
rng.shuffle(order)
lines = [str(n)]
for c, p, t in order:
    lines.append(f"{c} {p} {'almost protected' if t else 'protected'}")
sys.stdout.write("\n".join(lines) + "\n")
