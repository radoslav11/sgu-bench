"""Tests for p281 (Championship): two ratings over the same N distinct
team names (N <= 50000, names of <= 20 lowercase letters/digits).

Seed 1 is N=1, seed 2 a maximal random permutation (few huge blocks),
seed 3 maximal with identical ratings (50000 singleton blocks), later
seeds use planted block structure, reversed order and similar names.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

ALPHA = "abcdefghijklmnopqrstuvwxyz0123456789"
MAXN = 50000


def make_names(n, length=20):
    names = set()
    while len(names) < n:
        names.add("".join(rng.choice(ALPHA) for _ in range(rng.randint(1, length))))
    return list(names)


def emit(ord1, ord2):
    print(len(ord1))
    print("\n".join(ord1))
    print("\n".join(ord2))


if seed == 1:
    emit(["a"], ["a"])
elif seed == 2:
    names = make_names(MAXN)
    ord1 = names[:]
    rng.shuffle(ord1)
    ord2 = names[:]
    rng.shuffle(ord2)
    emit(ord1, ord2)
elif seed == 3:
    names = make_names(MAXN)
    emit(names, names)
elif seed == 4:
    # maximal with planted blocks of random sizes
    names = make_names(MAXN)
    ord1, ord2 = [], []
    i = 0
    while i < MAXN:
        size = min(rng.randint(1, 8), MAXN - i)
        block = names[i:i + size]
        b1, b2 = block[:], block[:]
        rng.shuffle(b1)
        rng.shuffle(b2)
        ord1 += b1
        ord2 += b2
        i += size
    emit(ord1, ord2)
elif seed == 5:
    # maximal, second rating is the reverse of the first (one giant block)
    names = make_names(MAXN)
    emit(names, names[::-1])
elif seed == 6:
    # similar names sharing long prefixes, mid size
    n = rng.randint(2, 5000)
    base = "team0prefix1shared"
    names = [base + format(i, "02d")[-2:] + str(i) for i in range(n)]
    names = list(dict.fromkeys(names))
    n = len(names)
    ord1 = names[:]
    rng.shuffle(ord1)
    ord2 = names[:]
    rng.shuffle(ord2)
    emit(ord1, ord2)
else:
    n = rng.randint(1, 2000)
    names = make_names(n)
    ord1 = names[:]
    rng.shuffle(ord1)
    ord2 = names[:]
    rng.shuffle(ord2)
    emit(ord1, ord2)
