"""Tests for p401 (Geologist Dubrovsky): N rivers, swim speed u, time t.

1 <= N <= 50; 1 <= u, t <= 1000; 1 <= w_i, v_i <= 1000. Seed 1 is the
minimal case, seeds 2-3 are maximal (N=50) with different feasibility
slack, seed 4 is infeasible (-1), seed 5 sits exactly on the feasibility
boundary, the rest are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, u, t, rivers):
    print(n, u, t)
    for w, v in rivers:
        print(w, v)


if seed == 1:
    emit(1, 1, 1, [(1, 1)])
elif seed == 2:
    # maximal everything, lots of spare time
    emit(50, 1000, 1000, [(1000, 1000)] * 50)
elif seed == 3:
    # maximal N with tight (but feasible) time budget and varied rivers
    rivers = [(rng.randint(1, 1000), rng.randint(1, 1000)) for _ in range(50)]
    total_w = sum(w for w, _ in rivers)
    u = total_w // 1000 + 1  # sum w_i / u < 1000 <= t
    emit(50, u, 1000, rivers)
elif seed == 4:
    # infeasible: sum w_i / u > t
    rivers = [(1000, rng.randint(1, 1000)) for _ in range(50)]
    emit(50, 1, 100, rivers)
elif seed == 5:
    # exactly on the feasibility boundary: sum w_i = u * t
    u, t = 10, 100
    rivers = [(100, rng.randint(1, 1000)) for _ in range(10)]
    emit(10, u, t, rivers)
else:
    n = rng.randint(1, 50)
    u = rng.randint(1, 1000)
    t = rng.randint(1, 1000)
    rivers = [(rng.randint(1, 1000), rng.randint(1, 1000)) for _ in range(n)]
    emit(n, u, t, rivers)
