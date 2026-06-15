"""Tests for p320 (The Influence of the Mafia): N x M digit grid
(N, M <= 500), threshold K <= 250000. Any digit grid is a valid map
because regions are exactly the 4-connected same-digit components.

Seed 1 is the 1x1 grid, seed 2 a maximal 500x500 random two-digit grid
with K=1, seed 3 maximal concentric rings (deep nesting of enclosing
zones), later seeds vary alphabet size, K, and blocky structures.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1, 1)
    print(5)
    sys.exit()

if seed == 2:
    n = m = 500
    print(n, m, 1)
    for _ in range(n):
        print("".join(rng.choice("01") for _ in range(m)))
    sys.exit()

if seed == 3:
    n = m = 500
    print(n, m, 900)
    for i in range(n):
        row = []
        for j in range(m):
            r = min(i, j, n - 1 - i, m - 1 - j)
            row.append(str(r % 2))
        print("".join(row))
    sys.exit()

if seed == 4:
    # maximal grid, larger alphabet, K excludes mid-size zones
    n = m = 500
    print(n, m, 40)
    for _ in range(n):
        print("".join(rng.choice("0123456789") for _ in range(m)))
    sys.exit()

n = rng.randint(1, 500)
m = rng.randint(1, 500)
k = rng.choice([1, 2, rng.randint(1, 20), rng.randint(1, n * m),
                n * m, 250000])
print(n, m, k)

style = seed % 3
if style == 0:
    alpha = rng.choice(["01", "012", "0123456789"])
    for _ in range(n):
        print("".join(rng.choice(alpha) for _ in range(m)))
elif style == 1:
    # blocky structure: random rectangles of digits stamped over a base
    grid = [[rng.choice("01") for _ in range(m)] for _ in range(n)]
    for _ in range(rng.randint(5, 60)):
        x1 = rng.randint(0, n - 1)
        x2 = rng.randint(x1, min(n - 1, x1 + rng.randint(0, 80)))
        y1 = rng.randint(0, m - 1)
        y2 = rng.randint(y1, min(m - 1, y1 + rng.randint(0, 80)))
        d = rng.choice("0123456789")
        for i in range(x1, x2 + 1):
            for j in range(y1, y2 + 1):
                grid[i][j] = d
    for row in grid:
        print("".join(row))
else:
    # concentric rings with random digits per ring (adjacent rings differ)
    digits = []
    prev = ""
    for _ in range(max(n, m)):
        d = rng.choice([c for c in "0123456789" if c != prev])
        digits.append(d)
        prev = d
    for i in range(n):
        row = []
        for j in range(m):
            r = min(i, j, n - 1 - i, m - 1 - j)
            row.append(digits[r])
        print("".join(row))
