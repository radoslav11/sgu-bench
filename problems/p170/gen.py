"""Tests for p170 (Particles): two "+"/"-" strings, lengths 1..5000.

Seed 1 is the one-character case. Seed 2 is maximal random with matching
counts; seed 3 is the worst-case block swap ("+"*2500+"-"*2500 vs its
mirror) maximizing the inversion count near 6.25M. Later seeds cover
count mismatch and length mismatch (-1 answers), all-equal strings, and
random shapes where b is a shuffle of a.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_str(n):
    return "".join(rng.choice("+-") for _ in range(n))


def shuffled(s):
    chars = list(s)
    rng.shuffle(chars)
    return "".join(chars)


if seed == 1:
    print("+")
    print("+")
elif seed == 2:
    a = rand_str(5000)
    print(a)
    print(shuffled(a))
elif seed == 3:
    print("+" * 2500 + "-" * 2500)
    print("-" * 2500 + "+" * 2500)
elif seed == 4:
    # same length, different counts -> -1
    a = "+" * 2500 + "-" * 2500
    b = "+" * 2501 + "-" * 2499
    print(shuffled(a))
    print(shuffled(b))
elif seed == 5:
    # different lengths -> -1
    print(rand_str(5000))
    print(rand_str(4999))
elif seed == 6:
    print("-" * 5000)
    print("-" * 5000)
else:
    n = rng.randint(1, 5000)
    a = rand_str(n)
    if seed % 2:
        b = shuffled(a)
    else:
        b = rand_str(rng.choice([n, n, n, rng.randint(1, 5000)]))
    print(a)
    print(b)
