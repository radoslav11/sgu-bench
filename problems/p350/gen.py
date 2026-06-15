"""Tests for p350 (XOR-omania): B is the multiset of all pairwise XORs of
an unknown set A of N numbers in [0, 2^31 - 1] whose nonzero elements are
linearly independent over GF(2); M = N(N-1)/2 <= 100, so N <= 14.

Each test is produced by sampling a linearly independent basis
{x1..x_{n-1}}, taking A = {0, x1, ..., x_{n-1}}, optionally XOR-shifting
the whole set by a constant (pairwise XORs are shift-invariant), and
shuffling the resulting B.

Seed 1 is the minimal instance M=1. Seeds 2 and 3 are maximal N=14
(M=91): seed 2 uses random dense 31-bit values, seed 3 builds A from the
13 low single-bit values so every B entry is tiny. Seed 4 is the
statement sample, later seeds are random sizes with mixed value styles.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

FULL = (1 << 31) - 1


def independent_set(k, value_fn):
    # Greedily collect k values that are linearly independent over GF(2).
    basis = []  # reduced echelon pivots
    vals = []
    while len(vals) < k:
        v = value_fn()
        r = v
        for b in basis:
            r = min(r, r ^ b)
        if r:
            basis.append(r)
            vals.append(v)
    return vals


def emit(n, xs, shift):
    a = [0] + xs
    a = [v ^ shift for v in a]
    b = []
    for i in range(n):
        for j in range(i + 1, n):
            b.append(a[i] ^ a[j])
    rng.shuffle(b)
    print(len(b))
    print(" ".join(map(str, b)))


if seed == 1:
    emit(2, [rng.randint(1, FULL)], 0)
elif seed == 2:
    xs = independent_set(13, lambda: rng.randint(1, FULL))
    emit(14, xs, rng.randint(0, FULL))
elif seed == 3:
    xs = [1 << i for i in range(13)]
    emit(14, xs, 0)
elif seed == 4:
    print(6)
    print("30 19 66 13 92 81")  # statement sample
else:
    n = rng.choice([2, 3, 4, 5, 6, 8, 10, 12, 13, 14])
    style = rng.choice(["small", "big", "bits"])
    if style == "small":
        fn = lambda: rng.randint(1, 1 << (n + 2))
    elif style == "big":
        fn = lambda: rng.randint(1, FULL)
    else:
        bits = rng.sample(range(31), n + 3)
        fn = lambda: sum(1 << b for b in rng.sample(bits, rng.randint(1, len(bits))))
    xs = independent_set(n - 1, fn)
    shift = 0 if rng.random() < 0.5 else rng.randint(0, FULL)
    emit(n, xs, shift)
