"""Tests for p526 (Running Hero): v<=1e5, 0<|G|<=1e5, n<=3000 stones.

Each stone: -1e5<=x1<=x2<=1e5, t in [1,1e5]. Seed 1 is the minimal case
(n=0). Seeds 2-3 are maximal n=3000: seed 2 packs many overlapping wide
stones along the corridor near (0,G) at many distinct times (the hard
case for interval propagation), seed 3 uses random stones at the full
coordinate range. Later seeds add: a negative goal, a tight wall that
forces waiting, degenerate point stones, and random mid-size cases.
"""

import random
import sys

COORD = 100000
TMAX = 100000
VMAX = 100000


def emit(v, g, stones):
    lines = [f"{v} {g} {len(stones)}"]
    for x1, x2, t in stones:
        lines.append(f"{x1} {x2} {t}")
    sys.stdout.write("\n".join(lines) + "\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(rng.randint(1, VMAX), 1, [])
    sys.exit()

if seed == 2:
    # Maximal: many wide overlapping stones across [0,G] at distinct times,
    # forcing dense interval propagation. Keep v moderate so timing matters.
    v = 100
    g = 100000
    n = 3000
    stones = []
    for i in range(n):
        t = rng.randint(1, TMAX)
        c = rng.randint(0, g)
        half = rng.randint(1, 5000)
        x1 = max(-COORD, c - half)
        x2 = min(COORD, c + half)
        if x1 > x2:
            x1, x2 = x2, x1
        stones.append((x1, x2, t))
    emit(v, g, stones)
    sys.exit()

if seed == 3:
    # Maximal: random stones over the full coordinate and time ranges.
    v = rng.randint(1, VMAX)
    g = rng.choice([-1, 1]) * rng.randint(1, COORD)
    n = 3000
    stones = []
    for _ in range(n):
        a = rng.randint(-COORD, COORD)
        b = rng.randint(-COORD, COORD)
        if a > b:
            a, b = b, a
        t = rng.randint(1, TMAX)
        stones.append((a, b, t))
    emit(v, g, stones)
    sys.exit()

if seed == 4:
    # Negative goal, moderate stones near the origin path.
    v = rng.randint(1, 1000)
    g = -rng.randint(1, COORD)
    n = rng.randint(1, 3000)
    stones = []
    for _ in range(n):
        c = rng.randint(g, 0)
        half = rng.randint(0, 3000)
        x1 = max(-COORD, c - half)
        x2 = min(COORD, c + half)
        t = rng.randint(1, TMAX)
        stones.append((x1, x2, t))
    emit(v, g, stones)
    sys.exit()

if seed == 5:
    # A nearly impassable wall: a wide stone covers the whole path at one
    # time, so Aladdin may need to be at an endpoint exactly. Lots of stones
    # at the same few times.
    v = rng.randint(1, 100)
    g = rng.randint(1, COORD)
    n = 3000
    stones = []
    walls_t = [rng.randint(1, TMAX) for _ in range(5)]
    for _ in range(n):
        t = rng.choice(walls_t)
        a = rng.randint(-COORD, COORD)
        b = rng.randint(-COORD, COORD)
        if a > b:
            a, b = b, a
        stones.append((a, b, t))
    emit(v, g, stones)
    sys.exit()

if seed == 6:
    # Degenerate point stones (x1 == x2) mixed with real ones; harmless.
    v = rng.randint(1, VMAX)
    g = rng.choice([-1, 1]) * rng.randint(1, COORD)
    n = rng.randint(0, 3000)
    stones = []
    for _ in range(n):
        c = rng.randint(-COORD, COORD)
        if rng.random() < 0.5:
            stones.append((c, c, rng.randint(1, TMAX)))
        else:
            a = rng.randint(-COORD, COORD)
            b = rng.randint(-COORD, COORD)
            if a > b:
                a, b = b, a
            stones.append((a, b, rng.randint(1, TMAX)))
    emit(v, g, stones)
    sys.exit()

if seed == 7:
    # Force infeasibility: a full-width wall lands before Aladdin can reach
    # either endpoint, and the goal lies past it, so the answer is -1.
    v = 1
    g = COORD
    n = rng.randint(1, 3000)
    stones = [(-COORD, COORD, 1)]
    for _ in range(n - 1):
        a = rng.randint(-COORD, COORD)
        b = rng.randint(-COORD, COORD)
        if a > b:
            a, b = b, a
        stones.append((a, b, rng.randint(1, TMAX)))
    emit(v, g, stones)
    sys.exit()

# Random mid-size.
v = rng.randint(1, VMAX)
g = rng.choice([-1, 1]) * rng.randint(1, COORD)
n = rng.randint(0, 1500)
stones = []
for _ in range(n):
    a = rng.randint(-COORD, COORD)
    b = rng.randint(-COORD, COORD)
    if a > b:
        a, b = b, a
    t = rng.randint(1, TMAX)
    stones.append((a, b, t))
emit(v, g, stones)
