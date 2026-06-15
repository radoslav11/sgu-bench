"""Tests for p267 (Optimist vs. Pessimist): N <= 1000 pies, K <= min(N, 10).

Each pie is a rectangle with integer corners (built from a lattice vector
(p, q) and its perpendicular, so rotated rectangles stay integral) plus two
integer candle points on the pie. Corners are emitted in shuffled order, as
in the official sample. Adversarial pies: candle at the center, candles on
one ray from the center, symmetric candles, candles on edges, equal
candles, and many equal-area pies to stress the tie threshold.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)
LIM = 999


def make_pie(kind, geom=None):
    while True:
        if geom is not None:
            p, q, s, t = geom
        else:
            p = rng.randint(-5, 5)
            q = rng.randint(-5, 5)
            if p == 0 and q == 0:
                continue
            s = rng.randint(4, 20) // 2 * 2
            t = rng.randint(4, 20) // 2 * 2
        ax = rng.randint(-500, 500)
        ay = rng.randint(-500, 500)

        def lat(i, j):
            return (ax + i * p - j * q, ay + i * q + j * p)

        corners = [lat(0, 0), lat(s, 0), lat(s, t), lat(0, t)]
        if any(abs(x) > LIM or abs(y) > LIM for x, y in corners):
            continue

        c = lat(s // 2, t // 2)
        if kind == "center":
            c1, c2 = c, lat(rng.randint(0, s), rng.randint(0, t))
            if rng.random() < 0.5:
                c1, c2 = c2, c1
        elif kind == "same_ray":
            c1, c2 = lat(s // 2 + 1, t // 2), lat(s // 2 + 2, t // 2)
        elif kind == "equal":
            c1 = lat(rng.randint(0, s), rng.randint(0, t))
            c2 = c1
        elif kind == "symmetric":
            di = rng.randint(0, s // 2 - 1) - s // 2
            dj = rng.randint(0, t // 2) - t // 2
            c1, c2 = lat(s // 2 + di, t // 2 + dj), lat(s // 2 - di, t // 2 - dj)
        elif kind == "edge":
            c1 = lat(rng.randint(0, s), 0)
            c2 = lat(rng.randint(0, s), t)
        else:
            c1 = lat(rng.randint(0, s), rng.randint(0, t))
            c2 = lat(rng.randint(0, s), rng.randint(0, t))

        order = corners[:]
        rng.shuffle(order)
        nums = [v for pt in order for v in pt] + [c1[0], c1[1], c2[0], c2[1]]
        return " ".join(map(str, nums))


KINDS = ["center", "same_ray", "equal", "symmetric", "edge",
         "random", "random", "random"]

if seed == 1:
    print(1, 0)
    print(make_pie("random", geom=(1, 0, 4, 4)))
elif seed == 2:
    n, k = 1000, 10
    print(n, k)
    for _ in range(n):
        print(make_pie(rng.choice(KINDS)))
elif seed == 3:
    # All pies congruent (every area ties at the threshold), mixed
    # cuttability; rotation varies but |(p,q)| and (s,t) are fixed.
    n, k = 1000, 10
    print(n, k)
    for _ in range(n):
        p, q = rng.choice([(3, 4), (4, 3), (5, 0), (0, 5), (-3, 4), (4, -3)])
        print(make_pie(rng.choice(KINDS), geom=(p, q, 6, 4)))
else:
    n = rng.randint(1, 200)
    k = rng.randint(0, min(n, 10))
    print(n, k)
    for _ in range(n):
        if rng.random() < 0.3:
            print(make_pie(rng.choice(KINDS), geom=(2, 1, 8, 6)))
        else:
            print(make_pie(rng.choice(KINDS)))
