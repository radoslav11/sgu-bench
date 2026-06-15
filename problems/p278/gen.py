"""Tests for p278 (Fuel): two-constraint LP, optimum is a mixture of at
most two fuel kinds (ray-vs-convex-hull of points (a/c, b/c)).

1 <= N <= 75000, 1 <= A, B <= 1000, fuel parameters in 1..100. Seed 1 is
minimal, seeds 2-3 maximal (random / heavily duplicated values). Other
seeds: collinear ratio points, skewed A vs B, one dominant fuel, a single
fuel kind repeated, mid-size random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(a_cap, b_cap, fuels):
    out = [f"{len(fuels)} {a_cap} {b_cap}"]
    out += [f"{a} {b} {c}" for a, b, c in fuels]
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, 1, [(1, 1, 1)])
elif seed == 2:
    fuels = [
        (rng.randint(1, 100), rng.randint(1, 100), rng.randint(1, 100))
        for _ in range(75000)
    ]
    emit(1000, 1000, fuels)
elif seed == 3:
    fuels = [
        (rng.randint(1, 10), rng.randint(1, 10), rng.randint(1, 100))
        for _ in range(75000)
    ]
    emit(rng.randint(1, 1000), rng.randint(1, 1000), fuels)
elif seed == 4:
    fuels = []
    for _ in range(75000):
        t = rng.randint(1, 50)
        fuels.append((t, 51 - t if rng.random() < 0.5 else t, 1))
    emit(1000, 500, fuels)
elif seed == 5:
    fuels = [
        (rng.randint(1, 100), rng.randint(1, 100), rng.randint(1, 100))
        for _ in range(75000)
    ]
    emit(1, 1000, fuels)
elif seed == 6:
    fuels = [(100, 100, rng.randint(1, 5)) for _ in range(74999)]
    fuels.append((1, 1, 100))
    rng.shuffle(fuels)
    emit(1000, 1, fuels)
elif seed == 7:
    emit(737, 211, [(37, 53, 71)] * 75000)
else:
    n = rng.randint(2, 500)
    fuels = [
        (rng.randint(1, 100), rng.randint(1, 100), rng.randint(1, 100))
        for _ in range(n)
    ]
    emit(rng.randint(1, 1000), rng.randint(1, 1000), fuels)
