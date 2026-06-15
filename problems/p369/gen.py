"""Tests for p369 (Game): closure of points under rectangle completion.

K <= 2*10^5 distinct black cells, coordinates within +-10^9. The answer
is sum over row/column connectivity components of rows x cols. Seed 1 is
the empty board, seed 2 a maximal dense grid patch (one huge component),
seed 3 a maximal staircase chain (deep DFS) plus random noise, later
seeds mix sparse and clustered patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXK = 200000
C = 10**9

pts = set()

if seed == 1:
    print(0)
    sys.exit()

if seed == 2:
    # Dense patch: 200000 cells from a 500x500 grid -> one big component.
    cells = [(x, y) for x in range(500) for y in range(500)]
    pts = set(rng.sample(cells, MAXK))
elif seed == 3:
    # Staircase chain of 150000 points (deep recursion in DFS solutions)
    # plus 50000 random far-away points.
    x0, y0 = -C, -C
    for i in range(75000):
        pts.add((x0 + i, y0 + i))
        pts.add((x0 + i + 1, y0 + i))
    while len(pts) < MAXK:
        pts.add((rng.randint(0, C), rng.randint(0, C)))
elif seed == 4:
    # Fully random in the whole range: mostly singleton components.
    while len(pts) < MAXK:
        pts.add((rng.randint(-C, C), rng.randint(-C, C)))
elif seed == 5:
    # Few distinct rows/columns: 450 x values, 450 y values.
    xs = rng.sample(range(-C, C), 450)
    ys = rng.sample(range(-C, C), 450)
    while len(pts) < MAXK:
        pts.add((rng.choice(xs), rng.choice(ys)))
else:
    k = rng.randint(1, MAXK)
    style = seed % 3
    if style == 1:
        k = min(k, 10000)  # only 101*101 distinct cells available
    while len(pts) < k:
        if style == 0:
            pts.add((rng.randint(-C, C), rng.randint(-C, C)))
        elif style == 1:
            pts.add((rng.randint(-50, 50), rng.randint(-50, 50)))
        else:
            cx = rng.choice([-C, 0, C - 1000])
            cy = rng.choice([-C, 0, C - 1000])
            pts.add((cx + rng.randint(0, 999), cy + rng.randint(0, 999)))

pts = list(pts)
rng.shuffle(pts)
print(len(pts))
out = [f"{x} {y}" for x, y in pts]
print("\n".join(out))
