"""Tests for p450 (Ramen Shop): seating simulation, average satisfaction.

N <= 100 counters with C_i <= 100 seats, M <= 10000 groups, strictly
increasing arrival times below the closing time T <= 10^9, P_i <=
max C_i, W_i, E_i <= 10^9. Seed 1 is the smallest shop. Seed 2 packs
10^4 groups into a tiny dense time window over full-size counters with
long eating times (heavy queueing, many give-ups). Seed 3 is max size
with everything random. Later seeds force simultaneous departures and
deadline ties, near-closing arrivals, W/E at 10^9 (timestamps past
2*10^9), and a single-counter crush.
"""

import random
import sys


def emit(n, t, caps, groups):
    m = len(groups)
    assert 1 <= n <= 100 and 1 <= m <= 10000 and 1 <= t <= 10**9
    mx = max(caps)
    prev = -1
    print(n, m, t)
    print(' '.join(map(str, caps)))
    for ti, p, w, e in groups:
        assert prev < ti < t and 1 <= p <= mx and 1 <= w <= 10**9
        assert 1 <= e <= 10**9
        prev = ti
        print(ti, p, w, e)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, 1, [1], [(0, 1, 1, 1)])
elif seed == 2:
    # max load, balanced contention: queueing without mass give-ups
    n, m = 100, 10000
    caps = [100] * n
    t = 10**9
    times = sorted(rng.sample(range(0, 350000), m))
    groups = [(ti, rng.randint(1, 100), rng.randint(500, 20000),
               rng.randint(1000, 20000)) for ti in times]
    emit(n, t, caps, groups)
elif seed == 3:
    # max size, random counters, moderate eating times
    n, m = 100, 10000
    caps = [rng.randint(1, 100) for _ in range(n)]
    t = 10**9
    times = sorted(rng.sample(range(0, 2 * 10**6), m))
    groups = [(ti, rng.randint(1, max(caps)), rng.randint(1, 50000),
               rng.randint(1, 30000) if rng.random() < 0.97
               else rng.randint(1, 10**9)) for ti in times]
    emit(n, t, caps, groups)
elif seed == 4:
    # synchronized departures: same eat time, deadline ties on purpose
    n, m = 5, 10000
    caps = [60, 50, 40, 30, 20]
    t = 200000
    times = sorted(rng.sample(range(0, t - 1), m))
    groups = []
    for ti in times:
        e = rng.choice([100, 200, 400])
        w = rng.choice([100, 200, 300, 400]) - ti % 2
        groups.append((ti, rng.randint(1, 60), max(w, 1), e))
    emit(n, t, caps, groups)
elif seed == 5:
    # W and E up to the 10^9 limit, arrivals near closing time
    n, m = 10, 10000
    caps = [rng.randint(50, 100) for _ in range(n)]
    t = 10**9
    times = sorted(rng.sample(range(t - 500000, t), m))
    groups = [(ti, rng.randint(1, 30),
               rng.choice([1, 10**9, rng.randint(1, 100000)]),
               rng.randint(1, 5000) if rng.random() < 0.98
               else rng.choice([1, 10**9])) for ti in times]
    emit(n, t, caps, groups)
elif seed == 6:
    # single small counter, everyone fights for it
    n, m = 1, 10000
    caps = [7]
    t = 3 * 10**6
    times = sorted(rng.sample(range(0, t - 1), m))
    groups = [(ti, rng.randint(1, 7), rng.randint(1, 2000),
               rng.randint(1, 1000)) for ti in times]
    emit(n, t, caps, groups)
else:
    n = rng.randint(1, 5)
    m = rng.randint(1, 40)
    caps = [rng.randint(1, 10) for _ in range(n)]
    t = rng.randint(2, 300)
    m = min(m, t - 1)
    times = sorted(rng.sample(range(0, t - 1), m))
    groups = [(ti, rng.randint(1, max(caps)), rng.randint(1, 50),
               rng.randint(1, 60)) for ti in times]
    emit(n, t, caps, groups)
