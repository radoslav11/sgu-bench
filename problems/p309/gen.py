"""Tests for p309 (Real Fun): n distinct points, cover with three d x d squares.

4 <= n <= 20000, |x|, |y| <= 10^9. Seed 1 is the minimal sample-like
case, seed 2 is maximal with uniform points over the full range, seed 3
is maximal with three tight far-apart clusters (the intended structure).
Other seeds: collinear points, diagonal, two clusters plus noise, a
dense grid, and random mid-size sets. Points are deduplicated.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
C = 10**9


def emit(points, n_target):
    pts = list(dict.fromkeys(points))
    while len(pts) < n_target:
        p = (rng.randint(-C, C), rng.randint(-C, C))
        if p not in set(pts):
            pts.append(p)
    pts = pts[:n_target]
    print(len(pts))
    for x, y in pts:
        print(x, y)


def cluster(cx, cy, r, k):
    return [(rng.randint(cx - r, cx + r), rng.randint(cy - r, cy + r)) for _ in range(k)]


if seed == 1:
    emit([(0, 1), (0, -1), (1, 0), (-1, 0)], 4)
elif seed == 2:
    emit([(rng.randint(-C, C), rng.randint(-C, C)) for _ in range(25000)], 20000)
elif seed == 3:
    pts = (
        cluster(-C + 10**7, -C + 10**7, 10**7, 8000)
        + cluster(C - 10**7, 0, 10**7, 8000)
        + cluster(0, C - 10**7, 10**7, 8000)
    )
    rng.shuffle(pts)
    emit(pts, 20000)
elif seed == 4:
    emit([(rng.randint(-C, C), 0) for _ in range(25000)], 20000)
elif seed == 5:
    pts = cluster(-C // 2, -C // 2, 10**8, 12000) + cluster(C // 2, C // 2, 10**8, 12000)
    rng.shuffle(pts)
    emit(pts, 20000)
elif seed == 6:
    pts = []
    while len(pts) < 25000:
        t = rng.randint(-C, C)
        pts.append((t, min(C, max(-C, t + rng.randint(-1000, 1000)))))
    emit(pts, 20000)
elif seed == 7:
    pts = [(x * 13_000_000, y * 13_000_000) for x in range(-70, 71) for y in range(-70, 71)]
    rng.shuffle(pts)
    emit(pts, 19000)
else:
    n = rng.randint(4, 5000)
    r = rng.choice([100, 10**6, C])
    emit([(rng.randint(-r, r), rng.randint(-r, r)) for _ in range(2 * n + 10)], n)
