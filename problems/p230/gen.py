"""Tests for p230 (Weighings): order N boxes from "P lighter than Q" facts.

N <= 100 boxes, M <= 10000 weighings. Consistent seeds draw the pairs
from a hidden permutation so a topological order exists; inconsistent
seeds plant a cycle (or a self-loop) so the answer is "No solution".
Seed 1 is the minimal N=1 case, seeds 2-3 are maximal M=10000 cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def consistent(n, m):
    perm = list(range(1, n + 1))
    rng.shuffle(perm)
    lines = []
    for _ in range(m):
        i, j = rng.sample(range(n), 2)
        if perm[i] > perm[j]:
            i, j = j, i
        lines.append(f"{i + 1} {j + 1}")
    print(n, m)
    print("\n".join(lines))


if seed == 1:
    print(1, 1)
    print(1, 1)  # box lighter than itself -> "No solution"
elif seed == 2:
    consistent(100, 10000)
elif seed == 3:
    # maximal size with a planted 3-cycle -> "No solution"
    n, m = 100, 10000
    lines = []
    a, b, c = rng.sample(range(1, n + 1), 3)
    lines += [f"{a} {b}", f"{b} {c}", f"{c} {a}"]
    while len(lines) < m:
        u, v = rng.sample(range(1, n + 1), 2)
        lines.append(f"{u} {v}")
    print(n, m)
    print("\n".join(lines))
elif seed == 4:
    # long chain 1 < 2 < ... < 100, duplicated edges up to M
    n, m = 100, 10000
    lines = [f"{i} {i + 1}" for i in range(1, n)]
    while len(lines) < m:
        i = rng.randint(1, n - 1)
        lines.append(f"{i} {i + 1}")
    print(n, m)
    print("\n".join(lines))
elif seed == 5:
    # n=2 with both orders -> 2-cycle, "No solution"
    print(2, 2)
    print(1, 2)
    print(2, 1)
elif seed == 6:
    # sparse consistent case: many incomparable boxes
    consistent(100, rng.randint(1, 60))
else:
    n = rng.randint(2, 100)
    m = rng.randint(1, 10000)
    if seed % 2 == 0:
        consistent(n, m)
    else:
        print(n, m)
        print("\n".join(
            f"{rng.randint(1, n)} {rng.randint(1, n)}" for _ in range(m)
        ))
