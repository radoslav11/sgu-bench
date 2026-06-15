"""Tests for p114 (Telecasting station): 0 < N < 15000, 0 < X, P < 50000.

Seed 1 is a single city, seed 2 the maximal N = 14999 with random
coordinates and populations, seed 3 maximal with two equal-weight
clusters at the ends (the whole middle is optimal), seed 4 many cities
on one coordinate, seed 5 an exact 50/50 split between two coordinates,
the rest random mid sizes with duplicate coordinates.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(cities):
    print(len(cities))
    out = []
    for x, p in cities:
        out.append(f"{x} {p}")
    print("\n".join(out))


if seed == 1:
    emit([(rng.randint(1, 49999), rng.randint(1, 49999))])
elif seed == 2:
    emit([(rng.randint(1, 49999), rng.randint(1, 49999))
          for _ in range(14999)])
elif seed == 3:
    cities = [(rng.randint(1, 100), 10) for _ in range(7000)]
    cities += [(rng.randint(49900, 49999), 10) for _ in range(7000)]
    cities += [(rng.randint(101, 49899), 1) for _ in range(999)]
    rng.shuffle(cities)
    emit(cities)
elif seed == 4:
    x = rng.randint(1, 49999)
    emit([(x, rng.randint(1, 49999)) for _ in range(5000)])
elif seed == 5:
    emit([(10, 7), (20, 7)])
else:
    n = rng.randint(2, 14999)
    span = rng.choice([50, 1000, 49999])
    emit([(rng.randint(1, span), rng.randint(1, 49999)) for _ in range(n)])
