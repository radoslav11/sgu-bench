"""Tests for p475 (Be a Smart Raftsman): n <= 10 participants (w, t, s),
m <= 1000 riffles (c, D, d), all values in [1, 10000].

Seed 1 is the minimal 1x1 case, seeds 2-3 are maximal n = 10, m = 1000
files; seed 2 uses uniform random values, seed 3 makes capsize
decisions tight (critical weights near subset sums of the weights,
expensive swaps, slow walkers). Later seeds cover degenerate patterns:
equal weights, always/never capsizing, walking dominating, and the
statement sample.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, m, people, riffles):
    print(n, m)
    for p in people:
        print(*p)
    for r in riffles:
        print(*r)


if seed == 1:
    emit(1, 1, [(1, 1, 1)], [(1, 1, 1)])
elif seed == 2:
    n, m = 10, 1000
    people = [
        (rng.randint(1, 10000), rng.randint(1, 10000), rng.randint(1, 10000))
        for _ in range(n)
    ]
    riffles = [
        (rng.randint(1, 10000), rng.randint(1, 10000), rng.randint(1, 10000))
        for _ in range(m)
    ]
    emit(n, m, people, riffles)
elif seed == 3:
    n, m = 10, 1000
    weights = [rng.randint(300, 1200) for _ in range(n)]
    people = [
        (weights[j], rng.randint(2000, 10000), rng.randint(500, 5000))
        for j in range(n)
    ]
    riffles = []
    for _ in range(m):
        k = rng.randint(1, n)
        subset = rng.sample(weights, k)
        crit = max(1, min(10000, sum(subset) + rng.randint(-50, 50)))
        slow = rng.randint(3000, 10000)
        fast = rng.randint(1, slow)
        riffles.append((crit, slow, fast))
    emit(n, m, people, riffles)
elif seed == 4:
    # statement sample
    emit(
        2,
        3,
        [(50, 5, 1), (70, 20, 1)],
        [(30, 15, 10), (60, 100, 10), (70, 100, 10)],
    )
elif seed == 5:
    # everyone identical, critical weight exactly at the subset boundary
    n, m = 10, 1000
    people = [(1000, 5000, 1000)] * n
    riffles = [
        (1000 * rng.randint(1, n), 9000, 1000) for _ in range(m)
    ]
    emit(n, m, people, riffles)
elif seed == 6:
    # raft always capsizes (c = 1 < any weight); walkers are fast
    n, m = 10, 1000
    people = [
        (rng.randint(2, 10000), rng.randint(1, 50), rng.randint(1, 10000))
        for _ in range(n)
    ]
    riffles = [
        (1, rng.randint(5000, 10000), rng.randint(1, 5000))
        for _ in range(m)
    ]
    emit(n, m, people, riffles)
elif seed == 7:
    # raft never capsizes; swaps are exorbitant, walkers are very slow
    n, m = 10, 1000
    people = [
        (rng.randint(1, 900), rng.randint(9000, 10000), 10000)
        for _ in range(n)
    ]
    riffles = [
        (10000, rng.randint(1, 10000), rng.randint(1, 100))
        for _ in range(m)
    ]
    emit(n, m, people, riffles)
else:
    n = rng.randint(1, 10)
    m = rng.randint(1, 1000)
    people = [
        (rng.randint(1, 2000), rng.randint(1, 100), rng.randint(1, 100))
        for _ in range(n)
    ]
    riffles = [
        (rng.randint(1, 2000 * n), rng.randint(1, 200), rng.randint(1, 200))
        for _ in range(m)
    ]
    emit(n, m, people, riffles)
