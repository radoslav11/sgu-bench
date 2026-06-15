"""Tests for p531 (Bonnie and Clyde): pick two banks >= d apart, max money.

Constraints: 1 <= n <= 2e5, 1 <= d <= 1e8, 1 <= xi, wi <= 1e8,
positions distinct, banks given in increasing xi order.

Seeds: 1 minimal, 2 and 3 maximal (different structure), rest random plus
adversarial (no-solution, ties, tight distances).
"""

import random
import sys

NMAX = 200000
COORD = 10**8
WMAX = 10**8


def emit(n, d, banks):
    out = [f"{n} {d}"]
    for x, w in banks:
        out.append(f"{x} {w}")
    sys.stdout.write("\n".join(out) + "\n")


def distinct_sorted_xs(n, rng, hi=COORD):
    """n distinct coords in [1, hi], returned sorted ascending."""
    if n > hi:
        n = hi
    xs = rng.sample(range(1, hi + 1), n)
    xs.sort()
    return xs


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    # Minimal: n=1, no pair possible -> -1 -1.
    emit(1, 1, [(5, 7)])
elif seed == 2:
    # Maximal: n=2e5 spread coords, full money range, moderate d.
    n = NMAX
    xs = distinct_sorted_xs(n, rng)
    banks = [(x, rng.randint(1, WMAX)) for x in xs]
    d = rng.randint(1, COORD)
    emit(n, d, banks)
elif seed == 3:
    # Maximal, dense consecutive coords (xs = 1..n), small d => almost all
    # pairs valid; forces many lower_bound hits at the boundary.
    n = NMAX
    banks = [(i, rng.randint(1, WMAX)) for i in range(1, n + 1)]
    d = rng.randint(1, n - 1)
    emit(n, d, banks)
elif seed == 4:
    # Maximal-ish: d larger than the whole span -> no valid pair (-1 -1).
    n = NMAX
    xs = distinct_sorted_xs(n, rng, hi=COORD // 2)
    banks = [(x, rng.randint(1, WMAX)) for x in xs]
    d = COORD  # span <= COORD//2 < d
    emit(n, d, banks)
elif seed == 5:
    # Tight: d exactly equals the maximum span so only the extreme pair works.
    n = 200000
    banks = [(i, rng.randint(1, WMAX)) for i in range(1, n + 1)]
    d = n - 1
    emit(n, d, banks)
elif seed == 6:
    # Many equal weights to exercise tie-breaking in prefix/suffix maxima.
    n = 150000
    xs = distinct_sorted_xs(n, rng)
    w = rng.randint(1, WMAX)
    banks = [(x, w) for x in xs]
    d = rng.randint(1, COORD // 2)
    emit(n, d, banks)
else:
    # Random mid-size with varied d (sometimes no solution).
    n = rng.randint(1, 5000)
    hi = rng.choice([n, n * 3, COORD])
    xs = distinct_sorted_xs(n, rng, hi=max(hi, n))
    span = xs[-1] - xs[0] if n > 1 else 0
    d = rng.randint(1, max(1, span + 5))
    banks = [(x, rng.randint(1, WMAX)) for x in xs]
    emit(n, d, banks)
