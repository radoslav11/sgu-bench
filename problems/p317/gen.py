"""Tests for p317 (Fast Ride): B in [1, 1e8], N in [1, 5000] stables, total
horses <= 1e5, stable positions Xi in [0, 1e8] (may repeat), horse speed v
in [1, 1e8], range d in [1, 1e8].

Seed 1 is the minimal case (one stable at 0, one horse that reaches B).
Seeds 2 and 3 are maximal: seed 2 has N=5000 stables filling the horse
budget with overlapping reachable ranges (heavy convex-hull-trick load),
seed 3 spreads 100000 horses across 5000 stables with short ranges so the
DP chains through nearly every stable. Seeds 4-5 are unsolvable / boundary
cases (gap with no horse spanning it, no stable at 0). The rest are
randomized medium cases, repeated positions, and many-horses-one-stable.

A stable at position 0 is included whenever the case is meant to be
solvable, otherwise -1 is the intended answer.
"""

import random
import sys

BMAX = 10**8
NMAX = 5000
HMAX = 10**5
VMAX = 10**8
DMAX = 10**8

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(B, stables):
    out = [f"{B} {len(stables)}"]
    for x, horses in stables:
        out.append(f"{x} {len(horses)}")
        for v, d in horses:
            out.append(f"{v} {d}")
    sys.stdout.write("\n".join(out) + "\n")


def rand_horse():
    return rng.randint(1, VMAX), rng.randint(1, DMAX)


if seed == 1:
    emit(1, [(0, [(1, 1)])])
elif seed == 2:
    # N=5000 stables at positions 0..B-step, each with 20 horses (total
    # 100000), ranges long enough to reach far ahead -> dense CHT updates.
    B = BMAX
    n = NMAX
    step = B // n
    stables = []
    per = HMAX // n
    for i in range(n):
        x = i * step
        horses = []
        for _ in range(per):
            v = rng.randint(1, VMAX)
            # range spans many stables ahead
            d = rng.randint(step, B)
            horses.append((v, d))
        stables.append((x, horses))
    emit(B, stables)
elif seed == 3:
    # 100000 horses over 5000 stables, short ranges so the messenger must
    # hop through almost every stable (long DP chain).
    B = BMAX
    n = NMAX
    step = B // n
    stables = []
    per = HMAX // n
    for i in range(n):
        x = i * step
        horses = []
        for _ in range(per):
            v = rng.randint(1, VMAX)
            # just enough to reach a few stables forward
            d = rng.randint(step, 3 * step)
            horses.append((v, d))
        stables.append((x, horses))
    emit(B, stables)
elif seed == 4:
    # Unsolvable: no stable at position 0.
    emit(100, [(10, [(5, 100)]), (20, [(3, 100)])])
elif seed == 5:
    # Unsolvable: a gap no horse can cross.
    emit(100, [(0, [(5, 10)]), (50, [(7, 100)])])
elif seed == 6:
    # Single stable at 0 with many horses (all budget in one stable).
    B = BMAX
    m = HMAX
    horses = [rand_horse() for _ in range(m)]
    # guarantee at least one can finish
    horses[0] = (rng.randint(1, VMAX), B)
    emit(B, [(0, horses)])
elif seed == 7:
    # Repeated positions: several stables share the same coordinate.
    B = 1000
    stables = [(0, [(2, 100)])]
    for _ in range(50):
        x = rng.choice([0, 100, 100, 200, 500, 500])
        horses = [(rng.randint(1, 100), rng.randint(50, 1000))
                  for _ in range(rng.randint(1, 5))]
        stables.append((x, horses))
    emit(B, stables)
elif seed == 8:
    # Mid random solvable.
    B = rng.randint(1, BMAX)
    n = rng.randint(2, 1000)
    positions = sorted({rng.randint(0, B) for _ in range(n)} | {0})
    stables = []
    budget = HMAX
    for x in positions:
        m = rng.randint(1, max(1, budget // len(positions)))
        m = max(1, m)
        horses = [(rng.randint(1, VMAX), rng.randint(1, DMAX))
                  for _ in range(m)]
        # bias one horse to a long range to make it likely solvable
        horses[0] = (rng.randint(1, VMAX), rng.randint(B // 4 + 1, BMAX))
        stables.append((x, horses))
        budget -= m
    emit(B, stables)
elif seed == 9:
    # Small fully random (may be -1).
    B = rng.randint(1, 1000)
    n = rng.randint(1, 30)
    stables = []
    for _ in range(n):
        x = rng.randint(0, 1000)
        m = rng.randint(1, 5)
        stables.append((x, [(rng.randint(1, 100), rng.randint(1, 1000))
                            for _ in range(m)]))
    emit(B, stables)
else:
    # Another maximal-ish solvable mix: clustered positions, varied speeds.
    B = BMAX
    n = NMAX
    stables = []
    per = HMAX // n
    positions = sorted({rng.randint(0, B) for _ in range(n - 1)} | {0})
    for x in positions:
        horses = []
        for _ in range(per):
            horses.append((rng.randint(1, VMAX), rng.randint(1, DMAX)))
        horses[0] = (rng.randint(1, VMAX), rng.randint(B // 2, BMAX))
        stables.append((x, horses))
    emit(B, stables)
