"""Tests for p290 (Defend the Milky Way): n <= 100 stars in 3D.

Each star line is "<name> x y z" where the name may itself contain
spaces (parsers must split from the right). Coordinates are integers in
[-10000, 10000]. Seed 1 is a single star; seeds 2-3 are n=100 with
200-char names, seed 2 in general position and seed 3 clustered on a
small grid (many coplanar/collinear groups and duplicate points).
Later seeds cover fully degenerate sets: coplanar, collinear, all-equal.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

NAME_CHARS = (
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    "0123456789!#$%&'()*+,-./:;<=>?@[]^_`{|}~ "
)


def make_name(i, max_len=200):
    length = rng.randint(1, max_len - 8)
    chars = [rng.choice(NAME_CHARS) for _ in range(length)]
    # no leading/trailing space; uniqueness via the index suffix
    chars[0] = rng.choice(NAME_CHARS.strip())
    body = "".join(chars).rstrip() or "Q"
    return f"{body}#{i}"


def emit(points):
    print(len(points))
    for i, (x, y, z) in enumerate(points):
        print(f"{make_name(i)} {x} {y} {z}")


C = 10000

if seed == 1:
    print(1)
    print(f"{make_name(0)} {-C} {C} {0}")
elif seed == 2:
    pts = set()
    while len(pts) < 92:
        pts.add((rng.randint(-C, C), rng.randint(-C, C), rng.randint(-C, C)))
    corners = [(sx * C, sy * C, sz * C) for sx in (-1, 1) for sy in (-1, 1)
               for sz in (-1, 1)]
    emit(list(pts) + corners)
elif seed == 3:
    pts = [(rng.randint(-2, 2), rng.randint(-2, 2), rng.randint(-2, 2))
           for _ in range(100)]
    emit(pts)
elif seed == 4:
    # all points in the plane z = x + y (2D-degenerate -> everyone counts)
    pts = []
    for _ in range(100):
        x, y = rng.randint(-5000, 5000), rng.randint(-5000, 5000)
        pts.append((x, y, x + y))
    emit(pts)
elif seed == 5:
    # collinear points on a long segment
    pts = [(3 * t - C, -2 * t + 50, t) for t in rng.sample(range(0, 3000), 100)]
    emit(pts)
elif seed == 6:
    # all stars at the same location
    pts = [(123, -456, 789)] * 100
    emit(pts)
elif seed == 7:
    # points on the surface and inside of an axis-aligned cube: many lie on
    # hull faces without being vertices
    pts = []
    for _ in range(100):
        x, y, z = (rng.randint(-100, 100) for _ in range(3))
        if rng.random() < 0.7:
            axis = rng.randrange(3)
            v = [x, y, z]
            v[axis] = rng.choice([-100, 100])
            x, y, z = v
        pts.append((x, y, z))
    emit(pts)
else:
    n = rng.randint(2, 100)
    pts = [(rng.randint(-30, 30), rng.randint(-30, 30), rng.randint(-30, 30))
           for _ in range(n)]
    emit(pts)
