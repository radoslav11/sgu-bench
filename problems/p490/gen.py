"""Tests for p490 (Figure and Spots): T<=100, 1<=H,W<=20, 1<=N<=200.

Seed 1 is a single minimal case. Seeds 2-3 are maximal (T=100) with large
grids and a spread of N values, including feasible and impossible ones. Later
seeds mix small grids, edge widths/heights of 1-2, and N at its limits.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(cases):
    print(len(cases))
    for h, w, n in cases:
        print(h, w, n)


if seed == 1:
    emit([(1, 1, 1)])
elif seed == 2:
    # Maximal: 100 cases on the largest grid, full range of N.
    cases = []
    for i in range(100):
        n = 1 + (i * 199) // 99
        cases.append((20, 20, n))
    emit(cases)
elif seed == 3:
    # Maximal with varied large grids and N near the feasibility boundary.
    cases = []
    for _ in range(100):
        h = rng.randint(10, 20)
        w = rng.randint(10, 20)
        n = rng.randint(1, 200)
        cases.append((h, w, n))
    emit(cases)
elif seed == 4:
    # Degenerate small dimensions where no interior exists.
    cases = []
    for h in range(1, 4):
        for w in range(1, 4):
            for n in (1, 2, 200):
                cases.append((h, w, n))
    emit(cases)
elif seed == 5:
    # Exactly at maximum feasible N for several grids (checkerboard bound).
    cases = []
    for h in (10, 14, 18, 20):
        for w in (10, 14, 18, 20):
            ih, iw = h - 2, w - 2
            total = ih * iw if ih > 0 and iw > 0 else 0
            mx = (total + 1) // 2
            for n in (max(1, mx), max(1, mx - 1), mx + 1):
                if 1 <= n <= 200:
                    cases.append((h, w, n))
    emit(cases)
else:
    cases = []
    for _ in range(rng.randint(1, 100)):
        h = rng.randint(1, 20)
        w = rng.randint(1, 20)
        n = rng.randint(1, 200)
        cases.append((h, w, n))
    emit(cases)
