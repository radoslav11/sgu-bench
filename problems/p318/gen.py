"""Tests for p318 (Grants): N resources, M users, each user lists a set
of distinct resources. 1 <= N, M <= 100, set sizes 0..N.

Seed 1 is minimal (1 resource, 1 user, empty wish set), seeds 2-3 are
maximal (every user requests all / a large random subset of resources),
later seeds mix duplicates of user wish sets and sparse requests.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1, 1)
    print(0)
    sys.exit()

if seed == 2:
    n, m = 100, 100
    print(n, m)
    full = list(range(1, n + 1))
    for _ in range(m):
        print(n, *full)
    sys.exit()

if seed == 3:
    n, m = 100, 100
    print(n, m)
    for _ in range(m):
        k = rng.randint(90, 100)
        items = rng.sample(range(1, n + 1), k)
        print(k, *items)
    sys.exit()

n = rng.randint(1, 100)
m = rng.randint(1, 100)
print(n, m)

if seed % 3 == 0:
    # few distinct wish patterns repeated by many users
    patterns = []
    for _ in range(rng.randint(1, 4)):
        k = rng.randint(0, n)
        patterns.append(rng.sample(range(1, n + 1), k))
    for _ in range(m):
        items = rng.choice(patterns)
        print(len(items), *items)
else:
    for _ in range(m):
        k = rng.randint(0, n)
        items = rng.sample(range(1, n + 1), k)
        print(k, *items)
