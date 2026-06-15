"""Tests for p540 (Traffic Lights): 1 <= n < 20000, 1 <= s <= 20000,
10 <= vmin <= vmax <= 50, distinct light positions 1..s-1.

Seed 1 is a single light. Seeds 2 and 3 pack n = 19999 distinct lights on
the maximal street (s = 20000), seed 2 with the widest speed window and
seed 3 with a narrow window and uniform cycles. Later seeds vary the
number of lights, the speed window, and the cycle/phase distribution,
including a clustered-position case.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 19999
MAXS = 20000


def light(rng):
    r = rng.randint(10, 20)
    g = rng.randint(10, 20)
    d = rng.randint(0, r + g - 1)
    return r, g, d


def emit(n, s, vmin, vmax, positions, lights):
    lines = [f"{n} {s} {vmin} {vmax}"]
    for x, (r, g, d) in zip(positions, lights):
        lines.append(f"{x} {r} {g} {d}")
    sys.stdout.write("\n".join(lines) + "\n")


if seed == 1:
    r, g, d = light(rng)
    emit(1, 1000, 10, 30, [500], [(r, g, d)])
    sys.exit()

if seed == 2:
    n = MAXN
    s = MAXS
    positions = list(range(1, s))
    rng.shuffle(positions)
    positions = positions[:n]
    lights = [light(rng) for _ in range(n)]
    emit(n, s, 10, 50, positions, lights)
    sys.exit()

if seed == 3:
    n = MAXN
    s = MAXS
    positions = list(range(1, s))[:n]
    lights = [(10, 10, rng.randint(0, 19)) for _ in range(n)]
    emit(n, s, 30, 31, positions, lights)
    sys.exit()

# Randomized mid-size tests.
s = rng.randint(2, MAXS)
n = rng.randint(1, min(MAXN, s - 1))
if seed % 3 == 0:
    # Clustered positions in a narrow band.
    band = rng.sample(range(1, s), n)
    positions = band
else:
    positions = rng.sample(range(1, s), n)

vmin = rng.randint(10, 50)
vmax = rng.randint(vmin, 50)
lights = [light(rng) for _ in range(n)]
emit(n, s, vmin, vmax, positions, lights)
