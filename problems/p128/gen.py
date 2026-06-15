"""Tests for p128 (Snake): 4 <= N <= 10000, coords in [-10000, 10000].

Valid snakes are built as a comb polygon: columns x = 0..m with a flat
bottom at y = ymin and teeth of varying heights, n = 2(m+1) points.
Seed 1 is the minimal square, seed 2 a maximal comb with random
heights, seed 3 a maximal comb whose heights repeat from a small set
(many collinear horizontal edges). Seeds 4+ cover odd N, random point
clouds (answer 0), a 100x100 full grid (parity holds but the pairing
splits into many cycles), and crossing rectangles.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def comb(m, height_pool, shuffle_rng):
    # columns 0..m, bottom at y = 0; n = 2 * (m + 1) points
    heights = []
    prev = 0
    for _ in range(m):
        h = shuffle_rng.choice(height_pool)
        while h == prev:
            h = shuffle_rng.choice(height_pool)
        heights.append(h)
        prev = h
    pts = [(0, 0), (m, 0), (0, heights[0]), (m, heights[-1])]
    for i in range(1, m):
        pts.append((i, heights[i - 1]))
        pts.append((i, heights[i]))
    shuffle_rng.shuffle(pts)
    return pts


def emit(pts):
    print(len(pts))
    for x, y in pts:
        print(x, y)


if seed == 1:
    emit([(0, 0), (0, 3), (3, 3), (3, 0)])
elif seed == 2:
    emit(comb(4999, list(range(1, 10001)), rng))
elif seed == 3:
    emit(comb(4999, list(range(1, 60)), rng))
elif seed == 4:
    pts = set()
    while len(pts) < 9999:
        pts.add((rng.randint(-10000, 10000), rng.randint(-10000, 10000)))
    emit(sorted(pts))
elif seed == 5:
    pts = set()
    while len(pts) < 10000:
        pts.add((rng.randint(-10000, 10000), rng.randint(-10000, 10000)))
    emit(sorted(pts))
elif seed == 6:
    pts = [(x, y) for x in range(100) for y in range(100)]
    rng.shuffle(pts)
    emit(pts)
elif seed == 7:
    # two crossing rectangles: every row/column count is even
    emit([(0, 0), (0, 2), (2, 0), (2, 2), (1, -1), (1, 3), (3, -1), (3, 3)])
else:
    m = rng.randint(2, 400)
    pts = comb(m, list(range(1, 10001)), rng)
    if seed % 2 == 0:
        # perturb one point to break the snake
        i = rng.randrange(len(pts))
        x, y = pts[i]
        pts[i] = (x, y + 17 if y + 17 <= 10000 else y - 17)
    emit(pts)
