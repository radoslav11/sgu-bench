"""Tests for p214 (Weird Dissimilarity): alphabet of distinct printable
ASCII characters (codes 33..126, so up to 94 of them), two strings of
length <= 2000 over it, and a |S| x |S| non-negative cost matrix.

Seed 1 is minimal (1 char, 1-letter strings). Seed 2 is maximal: full
94-char alphabet, both strings of length 2000, random costs. Seed 3 is
maximal with adversarial costs (d(c,c) > 0, strongly asymmetric). Seed 4
pairs a 2000-long string with a 1-char one. Seeds 5+ are random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

FULL = "".join(chr(c) for c in range(33, 127))


def emit(sigma, lam, mu, maxd, diag_zero):
    k = len(sigma)
    print(sigma)
    print(lam)
    print(mu)
    rows = []
    for i in range(k):
        row = [rng.randint(0, maxd) for _ in range(k)]
        if diag_zero:
            row[i] = 0
        rows.append(" ".join(map(str, row)))
    print("\n".join(rows))


if seed == 1:
    print("a")
    print("a")
    print("a")
    print(3)
    sys.exit()

if seed == 2:
    sigma = FULL
    lam = "".join(rng.choice(sigma) for _ in range(2000))
    mu = "".join(rng.choice(sigma) for _ in range(2000))
    emit(sigma, lam, mu, 10**6, True)
elif seed == 3:
    # nonzero diagonal and skewed costs; strings use few letters
    sigma = FULL
    pool = rng.sample(sigma, 5)
    lam = "".join(rng.choice(pool) for _ in range(2000))
    mu = "".join(rng.choice(pool) for _ in range(2000))
    emit(sigma, lam, mu, 50, False)
elif seed == 4:
    sigma = FULL[:30]
    lam = "".join(rng.choice(sigma) for _ in range(2000))
    mu = rng.choice(sigma)
    emit(sigma, lam, mu, 1000, False)
elif seed == 5:
    # identical strings but costly diagonal: matching itself is bad
    sigma = FULL[:10]
    lam = "".join(rng.choice(sigma) for _ in range(1500))
    emit(sigma, lam, lam, 20, False)
else:
    k = rng.randint(1, 94)
    sigma = "".join(rng.sample(FULL, k))
    lam = "".join(rng.choice(sigma) for _ in range(rng.randint(1, 500)))
    mu = "".join(rng.choice(sigma) for _ in range(rng.randint(1, 500)))
    emit(sigma, lam, mu, rng.choice([1, 100, 10**6]), rng.random() < 0.5)
