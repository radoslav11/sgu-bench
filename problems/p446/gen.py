"""Tests for p446 (Rotation Estimation): angle between two photos of the
same constellation.

n <= 1000 stars, |x|, |y| <= 100, pairwise distances > 1e-5. Photo 2 is
photo 1 rotated about its centroid, randomly permuted and translated.
Points live on a 0.001 grid inside a radius-70 disc so the min-distance
guarantee holds; coordinates are printed with 12 decimals so the exact
rotational match survives rounding. Seed 2 is a 1000-point regular
polygon (every alignment candidate passes the length filter), seed 3 a
random 1000-point cloud.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def make_cloud(n):
    # Distinct grid points (multiples of 0.001) within radius ~70 of origin.
    pts = set()
    while len(pts) < n:
        x = rng.randint(-49000, 49000)
        y = rng.randint(-49000, 49000)
        pts.add((x, y))
    return [(x / 1000.0, y / 1000.0) for x, y in pts]


def emit(pts1, theta):
    n = len(pts1)
    cx = sum(p[0] for p in pts1) / n
    cy = sum(p[1] for p in pts1) / n
    c, s = math.cos(theta), math.sin(theta)
    tx = rng.uniform(-25, 25)
    ty = rng.uniform(-25, 25)
    pts2 = []
    for x, y in pts1:
        dx, dy = x - cx, y - cy
        pts2.append((cx + tx + dx * c - dy * s, cy + ty + dx * s + dy * c))
    rng.shuffle(pts2)
    print(n)
    for x, y in pts1:
        print(f"{x:.12f} {y:.12f}")
    for x, y in pts2:
        print(f"{x:.12f} {y:.12f}")


if seed == 1:
    print(1)
    print("0.0 0.0")
    print("57.3 -41.2")
    sys.exit()

if seed == 2:
    # Maximal worst case: regular 1000-gon, all radii equal, so every
    # candidate alignment survives the length filter.
    n = 1000
    pts = []
    for i in range(n):
        a = 2 * math.pi * i / n
        pts.append((70 * math.cos(a), 70 * math.sin(a)))
    emit(pts, rng.uniform(0, 2 * math.pi))
    sys.exit()

if seed == 3:
    # Maximal random cloud.
    emit(make_cloud(1000), rng.uniform(0, 2 * math.pi))
    sys.exit()

if seed == 4:
    # Two stars only.
    emit(make_cloud(2), rng.uniform(0, 2 * math.pi))
    sys.exit()

if seed == 5:
    # Identical photos (rotation 0) up to permutation/translation.
    emit(make_cloud(500), 0.0)
    sys.exit()

if seed == 6:
    # Rotation by exactly pi.
    emit(make_cloud(500), math.pi)
    sys.exit()

if seed == 7:
    # Concentric rings: many stars share the same distance to centroid.
    pts = []
    used = set()
    for r in (20, 40, 60):
        for _ in range(250):
            while True:
                a = rng.uniform(0, 2 * math.pi)
                x = round(r * math.cos(a), 3)
                y = round(r * math.sin(a), 3)
                if (x, y) not in used:
                    used.add((x, y))
                    pts.append((x, y))
                    break
    emit(pts, rng.uniform(0, 2 * math.pi))
    sys.exit()

# Random mid-size clouds with random rotation.
emit(make_cloud(rng.randint(3, 300)), rng.uniform(0, 2 * math.pi))
