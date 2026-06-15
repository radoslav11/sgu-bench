"""Tests for p363 (Baggage Room). N people (arrival ki non-decreasing,
1..1000; service ti 1..100) choose among M windows the shortest queue,
ties broken by lowest window number; arrivals at the exact moment someone
finishes see that person already gone.

Seed plan:
- seed 1: N = 1, M = 1 (minimal).
- seed 2: N = 100, M = 100, dense arrivals with max service times.
- seed 3: N = 100, M = 1 (single long queue, departure times up to 10^4+).
- seed 4: N = 100, M = 100, arrivals engineered to coincide with finish
  moments (k equals previous person's departure) to hit the just-left rule.
- seed 5: N = 100, M = 3, all service times equal (many queue-length ties).
- seed 6: N = 100, M = 100, everyone arrives at moment 1.
- seeds 7+: random N, M, arrivals and services.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(m, people):
    print(len(people), m)
    for k, t in people:
        print(k, t)


if seed == 1:
    emit(1, [(1, 1)])
elif seed == 2:
    ks = sorted(rng.randint(1, 50) for _ in range(100))
    emit(100, [(k, rng.randint(90, 100)) for k in ks])
elif seed == 3:
    ks = sorted(rng.randint(1, 1000) for _ in range(100))
    emit(1, [(k, rng.randint(1, 100)) for k in ks])
elif seed == 4:
    people = []
    k = 1
    for i in range(100):
        t = rng.randint(1, 10)
        people.append((k, t))
        # Next person arrives exactly when this one would finish if served
        # immediately, probing the "just left the queue" rule.
        k = min(1000, k + t if rng.random() < 0.7 else k)
    emit(100, people)
elif seed == 5:
    ks = sorted(rng.randint(1, 200) for _ in range(100))
    emit(3, [(k, 7) for k in ks])
elif seed == 6:
    emit(100, [(1, rng.randint(1, 100)) for _ in range(100)])
else:
    n = rng.randint(1, 100)
    m = rng.randint(1, 100)
    ks = sorted(rng.randint(1, 1000) for _ in range(n))
    emit(m, [(k, rng.randint(1, 100)) for k in ks])
