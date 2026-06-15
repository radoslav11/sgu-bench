"""Tests for p445 (Dig or Climb): polyline terrain, walk at vw or tunnel
horizontally at vc.

1 <= n <= 1000, 1 <= vw, vc <= 10, -10000 <= xi, yi <= 10000, xi strictly
increasing. Seed 1 is minimal (n=1), seeds 2-3 maximal (random mountains
vs deep sawtooth that keeps the tunnel scans long), later seeds add flat
segments, plateaus and monotone slopes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, vw, vc, pts):
    print(n)
    print(vw, vc)
    for x, y in pts:
        print(x, y)


if seed == 1:
    emit(1, 1, 1, [(0, 0)])
    sys.exit()

if seed == 2:
    # Maximal: 1000 points, wild random mountains, fast tunneling.
    n = 1000
    xs = sorted(rng.sample(range(-10000, 10001), n))
    pts = [(x, rng.randint(-10000, 10000)) for x in xs]
    emit(n, 1, 10, pts)
    sys.exit()

if seed == 3:
    # Maximal with different structure: long descending staircase with
    # small notches, so backward/forward tunnel scans run far.
    n = 1000
    xs = sorted(rng.sample(range(-10000, 10001), n))
    pts = []
    for i, x in enumerate(xs):
        base = 10000 - (20000 * i) // (n - 1)
        pts.append((x, max(-10000, min(10000, base + rng.randint(-5, 5)))))
    emit(n, 10, 1, pts)
    sys.exit()

if seed == 4:
    # Sample-like small mountain chain.
    emit(6, 1, 2, [(0, 0), (50, 50), (100, 0), (150, 0), (200, 50), (250, 0)])
    sys.exit()

if seed == 5:
    # Flat segments at various heights (vertical-free but yj==yj1 cases).
    n = 400
    xs = sorted(rng.sample(range(-10000, 10001), n))
    pts = []
    y = 0
    for i, x in enumerate(xs):
        if i % 2 == 0:
            y = rng.choice([-100, 0, 100, 5000, -5000])
        pts.append((x, y))
    emit(n, rng.randint(1, 10), rng.randint(1, 10), pts)
    sys.exit()

if seed == 6:
    # Plateaus: many consecutive points share the same height.
    n = 800
    xs = sorted(rng.sample(range(-10000, 10001), n))
    pts = []
    y = rng.randint(-10000, 10000)
    for x in xs:
        if rng.random() < 0.3:
            y = rng.randint(-10000, 10000)
        pts.append((x, y))
    emit(n, rng.randint(1, 10), rng.randint(1, 10), pts)
    sys.exit()

if seed == 7:
    # Single huge V canyon then a huge mountain.
    n = 1000
    xs = sorted(rng.sample(range(-10000, 10001), n))
    pts = []
    for i, x in enumerate(xs):
        t = i / (n - 1)
        if t < 0.25:
            y = -int(40000 * t)
        elif t < 0.75:
            y = int(-10000 + 40000 * (t - 0.25))
        else:
            y = int(10000 - 40000 * (t - 0.75))
        pts.append((x, max(-10000, min(10000, y))))
    emit(n, 2, 9, pts)
    sys.exit()

# Random mid-size tests with random speeds.
n = rng.randint(2, 300)
xs = sorted(rng.sample(range(-10000, 10001), n))
pts = [(x, rng.randint(-10000, 10000)) for x in xs]
emit(n, rng.randint(1, 10), rng.randint(1, 10), pts)
