"""Tests for p201 (Non Absorbing DFA): alphabet of distinct lowercase
letters, K <= 1000 states, N <= 60.

Seed 1 is minimal (1 letter, 1 state, N=1). Seed 2 is maximal with sparse
non-absorbing edges, seed 3 maximal with dense non-absorbing edges (long
chains and cycles, answer often 0 for many characters). Seed 4 builds a
1000-deep non-absorbing chain to stress chain resolution. Seeds 5+ are
random mid-size automata.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(alpha, k, s, terms, phi, chi, n):
    print(alpha)
    print(k)
    print(s, len(terms), " ".join(map(str, terms)))
    for row in phi:
        print(" ".join(map(str, row)))
    for row in chi:
        print(" ".join(map(str, row)))
    print(n)


if seed == 1:
    emit("a", 1, 1, [1], [[1]], [[0]], 1)
    sys.exit()

if seed == 2:
    alpha = string.ascii_lowercase
    k, n, p_chi = 1000, 60, 0.15
elif seed == 3:
    alpha = string.ascii_lowercase
    k, n, p_chi = 1000, 60, 0.85
elif seed == 4:
    # one long non-absorbing chain per character ending in a consume
    alpha = string.ascii_lowercase[: rng.randint(2, 26)]
    k, n = 1000, 60
    m = len(alpha)
    phi = [[(u + 2 if u < k - 1 else 1) for _ in range(m)] for u in range(k)]
    chi = [[1] * m for _ in range(k - 1)] + [[0] * m]
    terms = sorted(rng.sample(range(1, k + 1), rng.randint(1, k // 2)))
    emit(alpha, k, 1, terms, phi, chi, n)
    sys.exit()
else:
    alpha = "".join(
        sorted(rng.sample(string.ascii_lowercase, rng.randint(1, 26)))
    )
    k = rng.randint(1, 1000)
    n = rng.randint(1, 60)
    p_chi = rng.random()

m = len(alpha)
phi = [[rng.randint(1, k) for _ in range(m)] for _ in range(k)]
chi = [
    [1 if rng.random() < p_chi else 0 for _ in range(m)] for _ in range(k)
]
terms = sorted(rng.sample(range(1, k + 1), rng.randint(1, k)))
emit(alpha, k, 1 if seed % 2 else rng.randint(1, k), terms, phi, chi, n)
