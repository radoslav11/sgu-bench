"""Tests for p493 (Illumination of Buildings): T<=100, 1<=N<=1000, sum of
N^2 over all cases <= 1e6, coordinates 1<=L<R<=1e9, 1<=H<=1e9. Buildings
within a case have pairwise-disjoint, non-touching x-intervals.

Seed 1 is one minimal building. Seed 2 is a single maximal case N=1000 with
monotone-increasing heights (a staircase forcing many lamps). Seed 3 packs
the N^2 budget into 100 cases of N=100 each with varied skylines. Later seeds
add adversarial shapes: equal heights, V-valleys, deep wells, and randoms.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 10 ** 9
MAXH = 10 ** 9


def disjoint_buildings(n, heights, rng, max_right=MAXC):
    """Build n buildings with disjoint, non-touching x-intervals on [1,max_right]."""
    # Reserve room: each building needs width >=1 and a gap >=1 after it.
    # Place them left to right with random small widths and gaps, then if the
    # span would exceed max_right, fall back to a tight unit-width packing.
    buildings = []
    x = 1
    span_needed = 3 * n + 1
    if span_needed <= max_right:
        for i in range(n):
            remaining = n - i
            slack = max_right - x - (3 * remaining)
            slack = max(0, slack)
            w = rng.randint(1, 1 + min(slack // 2, 1000))
            l = x
            r = l + w
            buildings.append((l, r, heights[i]))
            gap = rng.randint(1, 1 + min(slack - (w - 1), 1000)) if slack > 0 else 1
            x = r + gap
    else:
        for i in range(n):
            l = 2 * i + 1
            r = l + 1
            buildings.append((l, r, heights[i]))

    return buildings


def emit(cases):
    print(len(cases))
    for buildings in cases:
        print(len(buildings))
        for l, r, h in buildings:
            print(l, r, h)


if seed == 1:
    emit([[(1, 2, 1)]])
elif seed == 2:
    # Single maximal case: a strictly increasing staircase of 1000 buildings.
    n = 1000
    heights = [1 + i for i in range(n)]
    emit([disjoint_buildings(n, heights, rng)])
elif seed == 3:
    # 100 cases of N=100 (sum N^2 = 1e6) with random skylines.
    cases = []
    for _ in range(100):
        n = 100
        heights = [rng.randint(1, MAXH) for _ in range(n)]
        cases.append(disjoint_buildings(n, heights, rng))
    emit(cases)
elif seed == 4:
    # Single N=1000 case, all equal heights (flat skyline).
    n = 1000
    heights = [500000 for _ in range(n)]
    emit([disjoint_buildings(n, heights, rng)])
elif seed == 5:
    # Single N=1000 case: a decreasing staircase then increasing (valley).
    n = 1000
    half = n // 2
    heights = [half - i for i in range(half)] + [1 + i for i in range(n - half)]
    heights = [max(1, h) for h in heights]
    emit([disjoint_buildings(n, heights, rng)])
elif seed == 6:
    # Many tiny cases with random small N filling the budget loosely.
    cases = []
    budget = 10 ** 6
    used = 0
    while used < budget - 2000 and len(cases) < 100:
        n = rng.randint(1, 40)
        used += n * n
        heights = [rng.randint(1, MAXH) for _ in range(n)]
        cases.append(disjoint_buildings(n, heights, rng))
    emit(cases)
elif seed == 7:
    # Alternating tall/short (comb pattern), single large case.
    n = 1000
    heights = [MAXH if i % 2 == 0 else 1 for i in range(n)]
    emit([disjoint_buildings(n, heights, rng)])
else:
    cases = []
    budget = 10 ** 6
    used = 0
    t = rng.randint(1, 100)
    for _ in range(t):
        nmax = int((budget - used) ** 0.5)
        if nmax < 1:
            break
        n = rng.randint(1, min(1000, nmax))
        used += n * n
        heights = [rng.randint(1, MAXH) for _ in range(n)]
        cases.append(disjoint_buildings(n, heights, rng))
    if not cases:
        cases.append([(1, 2, 1)])
    emit(cases)
