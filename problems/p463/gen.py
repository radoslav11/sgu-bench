"""Tests for p463 (Walking around Berhattan).

Grid n x m (1..100) of digits 0..9, path of 1..500 chars from {L,R,M}
that never leaves the district. The path is built by a guarded random
walk over intersections (x in 0..m, y in 0..n), so it is always valid.

Seed 1 is the minimal 1x1 case, seed 2 a max-size random walk, seed 3 a
max-size all-9 grid with a tight loop repeatedly passing the same blocks.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def walk(n, m, length, m_bias):
    """Random valid path: dirs 0=E,1=S,2=W,3=N from NW corner facing E."""
    dx = [1, 0, -1, 0]
    dy = [0, 1, 0, -1]
    x, y, d = 0, 0, 0
    out = []
    while len(out) < length:
        if rng.random() < m_bias:
            nx, ny = x + dx[d], y + dy[d]
            if 0 <= nx <= m and 0 <= ny <= n:
                out.append("M")
                x, y = nx, ny
                continue
        if rng.random() < 0.5:
            out.append("L")
            d = (d + 3) % 4
        else:
            out.append("R")
            d = (d + 1) % 4
    return "".join(out)


if seed == 1:
    print(1, 1)
    print(rng.randint(0, 9))
    print("M")
elif seed == 2:
    n = m = 100
    print(n, m)
    for _ in range(n):
        print("".join(str(rng.randint(0, 9)) for _ in range(m)))
    print(walk(n, m, 500, 0.85))
elif seed == 3:
    n = m = 100
    print(n, m)
    for _ in range(n):
        print("9" * m)
    print(("MRMRMRMR" * 63)[:500])
elif seed == 4:
    n, m = 1, 100
    print(n, m)
    print("".join(str(rng.randint(0, 9)) for _ in range(m)))
    print(walk(n, m, 500, 0.9))
elif seed == 5:
    n, m = 100, 1
    print(n, m)
    for _ in range(n):
        print(rng.randint(0, 9))
    print(walk(n, m, 500, 0.9))
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    print(n, m)
    zero_heavy = seed % 2 == 0
    for _ in range(n):
        if zero_heavy:
            row = "".join(rng.choice("000123456789") for _ in range(m))
        else:
            row = "".join(str(rng.randint(0, 9)) for _ in range(m))
        print(row)
    print(walk(n, m, rng.randint(1, 500), rng.uniform(0.5, 0.95)))
