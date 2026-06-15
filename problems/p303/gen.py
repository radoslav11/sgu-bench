"""Tests for p303 (Great Berland Wall): min-cost separating wall.

Provinces are unit cells of a rectangular grid (cell side 2, so cell
centers are integer points strictly inside), optionally with a carved
staircase corner so the country is non-convex yet simply connected.
Segments are the grid edges adjacent to at least one kept cell; any two
share at most an endpoint. 5 <= N <= 300, |coords| < 10^4, 1 <= v <=
1000. Seed 2 is a maximal 11x12 grid with random weights, seed 3 a
maximal carved 12x12 grid with all weights equal (heavy ties).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def grid_test(rows, cols, wlo, whi, carve=0):
    # kept[i][j] = cell (row i, col j) belongs to the country; carve
    # removes the staircase {i + j < carve} which keeps the rest simply
    # connected with no enclaves.
    kept = {
        (i, j)
        for i in range(rows)
        for j in range(cols)
        if i + j >= carve
    }
    ox = rng.randint(-9000, 9000 - 2 * cols)
    oy = rng.randint(-9000, 9000 - 2 * rows)

    segs = []
    for i in range(rows + 1):  # horizontal edges y = 2i, x in [2j, 2j+2]
        for j in range(cols):
            if (i - 1, j) in kept or (i, j) in kept:
                segs.append((2 * j + ox, 2 * i + oy, 2 * j + 2 + ox, 2 * i + oy))
    for i in range(rows):  # vertical edges x = 2j, y in [2i, 2i+2]
        for j in range(cols + 1):
            if (i, j - 1) in kept or (i, j) in kept:
                segs.append((2 * j + ox, 2 * i + oy, 2 * j + ox, 2 * i + 2 + oy))
    assert 5 <= len(segs) <= 300, len(segs)

    c1, c2 = rng.sample(sorted(kept), 2)
    rng.shuffle(segs)
    print(len(segs))
    for x1, y1, x2, y2 in segs:
        if rng.random() < 0.5:
            x1, y1, x2, y2 = x2, y2, x1, y1
        print(x1, y1, x2, y2, rng.randint(wlo, whi))
    print(2 * c1[1] + 1 + ox, 2 * c1[0] + 1 + oy,
          2 * c2[1] + 1 + ox, 2 * c2[0] + 1 + oy)


if seed == 1:
    # Minimal N = 5: a square split into two triangles by a diagonal.
    print(5)
    print(0, 0, 4, 0, rng.randint(1, 1000))
    print(4, 0, 4, 4, rng.randint(1, 1000))
    print(4, 4, 0, 4, rng.randint(1, 1000))
    print(0, 4, 0, 0, rng.randint(1, 1000))
    print(4, 0, 0, 4, rng.randint(1, 1000))
    print(1, 1, 3, 3)
elif seed == 2:
    grid_test(11, 12, 1, 1000)  # 287 segments, random weights
elif seed == 3:
    grid_test(12, 12, 1, 1, carve=4)  # near-max, all weights tie
elif seed == 4:
    grid_test(1, 74, 1, 1000)  # 1-row strip: wall must enclose one cell
elif seed == 5:
    grid_test(7, 18, 1, 3, carve=5)  # carved, tiny weight range
elif seed == 6:
    grid_test(2, 2, 1, 1000)
elif seed == 7:
    grid_test(rng.randint(2, 6), rng.randint(2, 8), 1, 1000)
elif seed == 8:
    grid_test(rng.randint(3, 9), rng.randint(3, 11), 1, 10, carve=2)
else:
    grid_test(rng.randint(4, 11), rng.randint(4, 12), 1, 1000)
