"""Tests for p229 (Divide and conquer): N <= 20 grid, set Q of '1' cells
must be split into two parts equal up to 90-degree rotations and shifts.

Seed 1 is the minimal 1x1 grid, seed 2 a full 20x20 board of ones,
seed 3 a planted-YES 20x20 instance (rotated copy of a random blob),
seed 4 dense random 20x20, seed 5 an empty board, the rest mix random
densities, planted YES instances and near-miss NO instances.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, cells):
    print(n)
    for r in range(n):
        print("".join("1" if (r, c) in cells else "0" for c in range(n)))


def planted_yes(n, half_size):
    """Random blob S in the left half plus a rotated, shifted copy."""
    s = set()
    while len(s) < half_size:
        s.add((rng.randint(0, n - 1), rng.randint(0, n // 2 - 1)))
    k = rng.randint(0, 3)
    t = s
    for _ in range(k):
        t = {(c, -r) for r, c in t}
    mr = min(r for r, c in t)
    mc = min(c for r, c in t)
    h = max(r for r, c in t) - mr
    t = {(r - mr + rng.randint(0, max(0, n - 1 - h)), c - mc + n // 2)
         for r, c in t}
    return s | t


if seed == 1:
    emit(1, {(0, 0)})
elif seed == 2:
    emit(20, {(r, c) for r in range(20) for c in range(20)})
elif seed == 3:
    emit(20, planted_yes(20, 60))
elif seed == 4:
    emit(20, {(r, c) for r in range(20) for c in range(20)
              if rng.random() < 0.7})
elif seed == 5:
    emit(20, set())
elif seed == 6:
    # Planted YES, then one extra cell flipped -> usually NO with even
    # set size kept by flipping two cells.
    cells = planted_yes(20, 50)
    free = [(r, c) for r in range(20) for c in range(20)
            if (r, c) not in cells]
    cells |= set(rng.sample(free, 2))
    emit(20, cells)
else:
    n = rng.randint(2, 20)
    if rng.random() < 0.4 and n >= 4:
        emit(n, planted_yes(n, rng.randint(2, max(2, n * n // 8))))
    else:
        p = rng.choice([0.15, 0.35, 0.55])
        emit(n, {(r, c) for r in range(n) for c in range(n)
                 if rng.random() < p})
