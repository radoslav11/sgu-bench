"""Tests for p477 (Doors): n, m <= 5000 pass times up to 1e9, 1 <= d <= 1e9.

Seed 1 is minimal (n = m = 1), seeds 2-3 are maximal: seed 2 uses 5000
distinct gaps per door (maximizes candidate t values, worst case for the
two-pointer search), seed 3 uses tight clusters separated by huge gaps.
Later seeds mix random sizes, dense no-solution cases and tiny d.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(p, q, d):
    print(len(p), len(q), d)
    print(*p)
    print(*q)


def distinct_sorted(rng, count, lo, hi):
    vals = rng.sample(range(lo, hi + 1), count)
    vals.sort()
    return vals


if seed == 1:
    emit([1], [1], 1)
elif seed == 2:
    # All 4999 gaps distinct per door -> ~5000 candidate t values each.
    gaps_p = rng.sample(range(2, 200001), 4999)
    gaps_q = rng.sample(range(2, 200001), 4999)
    p, q = [rng.randint(1, 1000)], [rng.randint(1, 1000)]
    for g in gaps_p:
        p.append(p[-1] + g)
    for g in gaps_q:
        q.append(q[-1] + g)
    emit(p, q, rng.randint(1, 100))
elif seed == 3:
    # Tight clusters far apart; doors' clusters interleave.
    p, q = [], []
    pos = 1
    while len(p) < 5000 or len(q) < 5000:
        size = rng.randint(1, 50)
        target = p if rng.random() < 0.5 else q
        for _ in range(size):
            if len(target) < 5000:
                pos += rng.randint(1, 3)
                target.append(pos)
        pos += rng.randint(10**5, 10**6)
    emit(p, q, rng.randint(1, 50))
elif seed == 4:
    # Dense identical interleaved streams with d = 1: likely no solution.
    p = list(range(1, 5001))
    q = list(range(1, 5001))
    emit(p, q, 1)
elif seed == 5:
    # One door used once, the other at full size, huge d.
    q = distinct_sorted(rng, 5000, 1, 10**9)
    emit([rng.randint(1, 10**9)], q, 10**9)
else:
    n = rng.randint(1, 300)
    m = rng.randint(1, 300)
    hi = rng.choice([10**3, 10**6, 10**9])
    p = distinct_sorted(rng, n, 1, hi)
    q = distinct_sorted(rng, m, 1, hi)
    d = rng.choice([1, rng.randint(1, hi), 10**9])
    emit(p, q, d)
