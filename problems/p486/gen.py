"""Tests for p486 (Bulls and Cows): two four-digit strings, each with all
digits distinct (leading zeros allowed).

The input size is fixed at two four-digit numbers, so there is no size to
maximize; the seeds instead cover every structural extreme. Seed 1 is two
identical numbers (4 bulls), seeds 2-3 force the all-cows and zero-overlap
extremes, and later seeds are random distinct-digit pairs including ones
that exercise leading zeros and partial overlaps.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def distinct4():
    return "".join(str(d) for d in rng.sample(range(10), 4))


if seed == 1:
    s = distinct4()
    print(s)
    print(s)
elif seed == 2:
    # Same digit multiset, fully permuted: maximal cows, zero bulls.
    digits = rng.sample(range(10), 4)
    secret = "".join(str(d) for d in digits)
    perm = digits[:]
    while any(perm[i] == digits[i] for i in range(4)):
        rng.shuffle(perm)

    print(secret)
    print("".join(str(d) for d in perm))
elif seed == 3:
    # Disjoint digit sets: zero bulls, zero cows.
    pool = rng.sample(range(10), 8)
    print("".join(str(d) for d in pool[:4]))
    print("".join(str(d) for d in pool[4:]))
elif seed == 4:
    # Leading zeros in both numbers.
    a = [0] + rng.sample(range(1, 10), 3)
    b = [0] + rng.sample(range(1, 10), 3)
    print("".join(str(d) for d in a))
    print("".join(str(d) for d in b))
elif seed == 5:
    # Some bulls, some cows: share digits with one fixed position.
    secret = rng.sample(range(10), 4)
    guess = secret[:]
    rng.shuffle(guess)
    print("".join(str(d) for d in secret))
    print("".join(str(d) for d in guess))
else:
    print(distinct4())
    print(distinct4())
