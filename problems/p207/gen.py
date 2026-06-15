"""Tests for p207 (Robbers): N <= 1000 robbers, M, Y <= 10000, sum X = Y.

X values are a random composition of Y into N positive parts. Seed 1 is
the minimal 1 1 1 case, seed 2 the maximal N=1000, M=Y=10000 random
composition, seed 3 maximal with all X equal (every fractional part tied).
Later seeds are random sizes, odd ones skewed so one robber holds most
of Y.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def composition(total, parts):
    cuts = sorted(rng.sample(range(1, total), parts - 1)) if parts > 1 else []
    prev = 0
    xs = []
    for c in cuts + [total]:
        xs.append(c - prev)
        prev = c
    return xs


if seed == 1:
    print(1, 1, 1)
    print(1)
    sys.exit()

if seed == 2:
    n, m, y = 1000, 10000, 10000
    xs = composition(y, n)
elif seed == 3:
    n, m, y = 1000, 9999, 10000
    xs = [10] * n
else:
    y = rng.randint(1, 10000)
    n = rng.randint(1, min(1000, y))
    m = rng.randint(1, 10000)
    if seed % 2 == 1 and y > n:
        xs = [1] * (n - 1) + [y - (n - 1)]
        rng.shuffle(xs)
    else:
        xs = composition(y, n)

print(n, m, y)
print(" ".join(map(str, xs)))
