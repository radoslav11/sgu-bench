"""Tests for p379 (Elevator): N <= 100, C, P, T <= 10^9, Ai >= 0 with
sum(Ai) <= 10^9.

Seed 1 minimal, seed 2 maximal (sum 10^9 on 100 floors, generous T),
seed 3 maximal with everyone on the top floor and tight T. Later seeds
randomize capacity/time scales: C=1 shuttles, huge T (everyone saved),
T too small for anything, and balanced random cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def random_partition(total, parts):
    cuts = sorted(rng.randint(0, total) for _ in range(parts - 1))
    vals = []
    prev = 0
    for c in cuts:
        vals.append(c - prev)
        prev = c
    vals.append(total - prev)
    return vals


if seed == 1:
    print(1, 1, 1, 1)
    print(0)
elif seed == 2:
    a = random_partition(10**9, 100)
    print(100, rng.randint(1, 1000), rng.randint(1, 100), 10**9)
    print(" ".join(map(str, a)))
elif seed == 3:
    a = [0] * 99 + [10**9]
    print(100, 10**9, 10**9, 10**9)
    print(" ".join(map(str, a)))
elif seed == 4:
    # capacity 1, lots of small trips
    a = [rng.randint(0, 100) for _ in range(100)]
    print(100, 1, 1, rng.randint(1, 10**6))
    print(" ".join(map(str, a)))
elif seed == 5:
    # T enormous relative to load: everyone is saved
    n = rng.randint(1, 100)
    a = [rng.randint(0, 1000) for _ in range(n)]
    print(n, rng.randint(1, 10), 1, 10**9)
    print(" ".join(map(str, a)))
elif seed == 6:
    # T smaller than one round trip to floor 1
    n = rng.randint(1, 100)
    a = [rng.randint(1, 10**6) for _ in range(n)]
    print(n, rng.randint(1, 10**9), 10**9, 1)
    print(" ".join(map(str, a)))
else:
    n = rng.randint(1, 100)
    total = rng.randint(0, 10**9)
    a = random_partition(total, n) if n > 1 else [total]
    print(
        n,
        rng.choice([1, rng.randint(1, 1000), 10**9]),
        rng.choice([1, rng.randint(1, 1000), 10**9]),
        rng.randint(1, 10**9),
    )
    print(" ".join(map(str, a)))
