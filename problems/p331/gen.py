"""Tests for p331 (Traffic Jam): N <= 5, d <= 1000, 0.001 <= c <= 1000.

Lane i moves at b_i + a_i*sin(t + delta_i) with integer 0 <= a_i < b_i <=
100 and delta_i in [0, 2*pi). Seed 1 is the minimal single-lane case.
Seed 2 is maximal with fast strongly-oscillating lanes (a=99, b=100) and
tiny switch cost. Seed 3 is maximal with slow lanes (a=1, b=2) so the
trip spans many sine periods and the optimum uses many lane changes.
Later seeds mix huge switch costs (never switch), constant-speed lanes
(a=0), and random parameters.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

TWO_PI = 2 * math.pi


def emit(n, d, c, lanes):
    print(n, d, "%.6f" % c)
    for a, b, delta in lanes:
        print(a, b, "%.15f" % delta)


if seed == 1:
    emit(1, 1, 0.001, [(0, 1, 0.0)])
elif seed == 2:
    lanes = [(99, 100, i * TWO_PI / 5) for i in range(5)]
    emit(5, 1000, 0.001, lanes)
elif seed == 3:
    lanes = [(1, 2, i * TWO_PI / 5) for i in range(5)]
    emit(5, 1000, 0.01, lanes)
elif seed == 4:
    lanes = [(99, 100, i * TWO_PI / 5) for i in range(5)]
    emit(5, 1000, 1000.0, lanes)  # switching never pays off
elif seed == 5:
    lanes = [(0, b, 0.0) for b in (1, 25, 50, 75, 100)]
    emit(5, 1000, 0.5, lanes)  # constant speeds, one obvious target lane
elif seed == 6:
    lanes = [(50, 51, i * TWO_PI / 5) for i in range(5)]
    emit(5, 1000, 0.1, lanes)  # deep speed dips, moderate switch cost
else:
    n = rng.randint(1, 5)
    d = rng.randint(1, 1000)
    c = rng.choice([0.001, 0.01, 0.5, 3.0, 100.0]) * rng.uniform(0.5, 2.0)
    c = min(1000.0, max(0.001, c))
    lanes = []
    for _ in range(n):
        b = rng.randint(1, 100)
        a = rng.randint(0, b - 1)
        lanes.append((a, b, rng.uniform(0, TWO_PI - 1e-9)))
    emit(n, d, c, lanes)
