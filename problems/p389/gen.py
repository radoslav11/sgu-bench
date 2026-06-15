"""Tests for p389 (Strange Planet): three n-bit strings (the capitals).

The statement's n bound is lost; the reference is O(n), so n = 10^6 is
used for the maximal tests. Seed 1 is n = 1, seed 2 a uniform random
maximal test, seed 3 a maximal test with structured group sizes (almost
all coordinates have all three capitals equal, which maximizes the
2^x0 free factor). Other seeds mix sizes and biased bit patterns,
including capitals that coincide.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(caps):
    for c in caps:
        print("".join(c))


if seed == 1:
    emit([["0"], ["1"], ["1"]])
elif seed == 2:
    n = 10**6
    emit([[rng.choice("01") for _ in range(n)] for _ in range(3)])
elif seed == 3:
    n = 10**6
    a, b, c = [], [], []
    for _ in range(n):
        bit = rng.choice("01")
        if rng.random() < 0.9:
            a.append(bit)
            b.append(bit)
            c.append(bit)
        else:
            lone = rng.randrange(3)
            row = [bit] * 3
            row[lone] = "1" if bit == "0" else "0"
            a.append(row[0])
            b.append(row[1])
            c.append(row[2])
    emit([a, b, c])
elif seed == 4:
    n = 200000
    a = [rng.choice("01") for _ in range(n)]
    emit([a, a[:], a[:]])
else:
    n = rng.randint(1, 300000)
    p_same = rng.random()
    a, b, c = [], [], []
    for _ in range(n):
        bit = rng.choice("01")
        if rng.random() < p_same:
            a.append(bit)
            b.append(bit)
            c.append(bit)
        else:
            lone = rng.randrange(3)
            row = [bit] * 3
            row[lone] = "1" if bit == "0" else "0"
            a.append(row[0])
            b.append(row[1])
            c.append(row[2])
    emit([a, b, c])
