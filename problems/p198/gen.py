"""Tests for p198 (Get Out!): N <= 300 circles (islands), then the ship
circle. Output YES/NO depending on whether the ship is enclosed by a chain
of overlapping islands.

All geometric decisions are kept far away from the 1e-6 tolerance: ring
circles overlap by a wide margin, escape gaps are tens of units wide, and
random seeds reject any pair within 0.01 of tangency (after inflating by
the ship radius) and any island within 0.05 of touching the ship start.

Seed 1: minimal N=1 (YES). Seed 2: max-size closed ring of 300 circles
(NO). Seed 3: max-size broken ring, 60-degree opening, plus decoys (YES).
Seed 4: two nested closed rings (NO). Seed 5: spiral winding twice around
the ship but never closing (YES). Seeds 6+: random scatter, answer decided
by the oracles.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(circles, ship):
    print(len(circles))
    for x, y, r in circles:
        print(f"{x:.3f} {y:.3f} {r:.3f}")
    print(f"{ship[0]:.3f} {ship[1]:.3f} {ship[2]:.3f}")


def ring(cx, cy, R, count, r, theta0=0.0):
    res = []
    for i in range(count):
        t = theta0 + 2 * math.pi * i / count
        res.append((cx + R * math.cos(t), cy + R * math.sin(t), r))
    return res


if seed == 1:
    emit([(50.0, 50.0, 3.0)], (0.0, 0.0, 1.0))
elif seed == 2:
    emit(ring(0, 0, 100, 300, 1.5), (0.0, 0.0, 1.0))
elif seed == 3:
    # 290 circles along a 300-degree arc -> wide opening; 10 decoys far away.
    circles = []
    for i in range(290):
        t = math.radians(30 + 300.0 * i / 289)
        circles.append((100 * math.cos(t), 100 * math.sin(t), 1.5))
    for i in range(10):
        circles.append((500.0 + 10 * i, 500.0, 2.0))
    emit(circles, (0.0, 0.0, 1.0))
elif seed == 4:
    circles = ring(0, 0, 60, 150, 2.0) + ring(0, 0, 120, 150, 3.5, 0.01)
    emit(circles, (0.0, 0.0, 1.0))
elif seed == 5:
    # Spiral: two full turns, channel between turns ~25 units wide.
    circles = []
    steps = 300
    for i in range(steps):
        t = 4 * math.pi * i / steps
        R = 30 + 25 * t / (2 * math.pi)
        circles.append((R * math.cos(t), R * math.sin(t), 2.0))
    emit(circles, (0.0, 0.0, 0.5))
else:
    n = rng.randint(2, 300)
    cr = round(rng.uniform(0.2, 4.0), 3)
    circles = []
    span = rng.choice([60, 150, 400])
    rad_hi = rng.choice([5.0, 15.0, 40.0])
    while len(circles) < n:
        x = round(rng.uniform(-span, span), 3)
        y = round(rng.uniform(-span, span), 3)
        r = round(rng.uniform(0.5, rad_hi), 3)
        # Ship starts at the origin: keep it strictly in free area.
        if math.hypot(x, y) < r + cr + 0.05:
            continue
        # Reject anything within 0.01 of tangency with an earlier circle
        # (after both are inflated by the ship radius).
        ok = True
        for px, py, pr in circles:
            d = math.hypot(x - px, y - py)
            if abs(d - (r + pr + 2 * cr)) < 0.01:
                ok = False
                break
        if ok:
            circles.append((x, y, r))
    emit(circles, (0.0, 0.0, cr))
