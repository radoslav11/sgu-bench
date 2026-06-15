"""Tests for p171 (Sarov zones): K <= 100 zones with capacities summing
to N <= 16000, zone levels Q and student levels P in 0..10^6, weights
w in 0..10^5.

Seed 1 is K=1 with zero students. Seeds 2-3 are maximal N=16000, K=100
(uniform random vs. heavy P==Q ties with equal weights, which punish
wrong strict-inequality handling). Later seeds include one giant zone,
the all-zero case where nobody qualifies, threshold-adjacent levels, and
random shapes including zero-capacity zones.
"""

import random
import sys


def emit(caps, qs, ps, ws):
    print(len(caps))
    print(" ".join(map(str, caps)))
    print(" ".join(map(str, qs)))
    print(" ".join(map(str, ps)))
    print(" ".join(map(str, ws)))


def split_caps(rng, k, total):
    cuts = sorted(rng.randint(0, total) for _ in range(k - 1))
    caps = []
    prev = 0
    for c in cuts + [total]:
        caps.append(c - prev)
        prev = c
    return caps


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit([0], [rng.randint(0, 10**6)], [], [])
elif seed == 2:
    k, n = 100, 16000
    emit(split_caps(rng, k, n),
         [rng.randint(0, 10**6) for _ in range(k)],
         [rng.randint(0, 10**6) for _ in range(n)],
         [rng.randint(0, 10**5) for _ in range(n)])
elif seed == 3:
    # few distinct levels shared by zones and students; equal weights
    k, n = 100, 16000
    levels = [rng.randint(0, 10**6) for _ in range(8)]
    emit(split_caps(rng, k, n),
         [rng.choice(levels) for _ in range(k)],
         [rng.choice(levels) for _ in range(n)],
         [1000] * n)
elif seed == 4:
    # one giant zone
    n = 16000
    emit([n], [500000],
         [rng.randint(0, 10**6) for _ in range(n)],
         [rng.randint(0, 10**5) for _ in range(n)])
elif seed == 5:
    # everyone at level 0: nobody beats any zone
    k, n = 100, 16000
    emit(split_caps(rng, k, n), [0] * k, [0] * n,
         [rng.randint(0, 10**5) for _ in range(n)])
elif seed == 6:
    # student levels sit exactly at Q and Q+1 for random zones
    k, n = 100, 16000
    qs = sorted(rng.randint(0, 10**6 - 1) for _ in range(k))
    ps = [rng.choice(qs) + rng.randint(0, 1) for _ in range(n)]
    emit(split_caps(rng, k, n), qs, ps,
         [rng.randint(0, 10**5) for _ in range(n)])
else:
    k = rng.randint(1, 100)
    n = rng.randint(0, 16000)
    emit(split_caps(rng, k, n) if k > 1 else [n],
         [rng.randint(0, 10**6) for _ in range(k)],
         [rng.randint(0, 10**6) for _ in range(n)],
         [rng.randint(0, 10**5) for _ in range(n)])
