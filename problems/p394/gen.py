"""Tests for p394 (Berhatton): 2 <= N <= 10^5, 1 <= K <= N-1,
0 <= x, y <= 10^9, 1 <= w <= 10^9.

Hut A closes when boys from at least K other huts reach it within their
range w (Manhattan). Seeds cover the max-N random case, a dense cluster
where every diamond overlaps many centers (boundary distances |d| == w
are frequent), identical points, and a diagonal line layout.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

C = 10**9
NMAX = 10**5


def emit(k, huts):
    out = [f"{len(huts)} {k}"]
    out.extend(f"{x} {y} {w}" for x, y, w in huts)
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, [(0, 0, 1), (1, 0, 1)])
elif seed == 2:
    # Max N, fully random; large w values make ranges overlap a lot.
    huts = [(rng.randint(0, C), rng.randint(0, C), rng.randint(1, C))
            for _ in range(NMAX)]
    emit(rng.randint(2, 50), huts)
elif seed == 3:
    # Max N in a tight 2000x2000 cluster with small w: many exact
    # boundary hits and moderate counts around K.
    huts = [(rng.randint(0, 2000), rng.randint(0, 2000), rng.randint(1, 60))
            for _ in range(NMAX)]
    emit(40, huts)
elif seed == 4:
    # All huts at one point: everyone reaches everyone.
    huts = [(C // 2, C // 2, 1) for _ in range(NMAX)]
    emit(NMAX - 1, huts)
elif seed == 5:
    # Diagonal line: transformed coordinates degenerate to one axis.
    huts = []
    for i in range(NMAX):
        t = rng.randint(0, 10**6)
        huts.append((t, t, rng.randint(1, 1000)))
    emit(rng.randint(1, 100), huts)
elif seed == 6:
    # Extreme K = N-1 with random points: closures need full coverage.
    huts = [(rng.randint(0, 1000), rng.randint(0, 1000), rng.randint(1, 2000))
            for _ in range(NMAX)]
    emit(NMAX - 1, huts)
else:
    n = rng.randint(2, 3000)
    span = rng.choice([10, 1000, C])
    wmax = rng.choice([1, 3, span, C])
    huts = [(rng.randint(0, span), rng.randint(0, span),
             rng.randint(1, wmax)) for _ in range(n)]
    emit(rng.randint(1, n - 1), huts)
