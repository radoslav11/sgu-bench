"""Tests for p268 (Hyper Almost Permutative String): 2 <= N <= 26.

Input is tiny by nature (two permutations of the first N letters).
Seed 1 is the minimal N=2 case. Seeds 2 and 3 are maximal N=26 cases
(random pair, and reversed-permutation pair which maximises the edit
distance between the windows). Other seeds cover rotations, single
swaps, and random N values.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, s1, s2):
    print(n)
    print(s1)
    print(s2)


def alphabet(n):
    return list(string.ascii_uppercase[:n])


if seed == 1:
    emit(2, "AB", "BA")
elif seed == 2:
    n = 26
    a = alphabet(n)
    s1 = "".join(rng.sample(a, n))
    while True:
        s2 = "".join(rng.sample(a, n))
        if s2 != s1:
            break
    emit(n, s1, s2)
elif seed == 3:
    n = 26
    a = alphabet(n)
    s1 = "".join(rng.sample(a, n))
    emit(n, s1, s1[::-1])
elif seed == 4:
    n = 26
    a = alphabet(n)
    s1 = "".join(a)
    emit(n, s1, s1[1:] + s1[0])  # rotation by one
elif seed == 5:
    n = 26
    a = alphabet(n)
    s1 = "".join(a)
    s2 = list(a)
    s2[0], s2[1] = s2[1], s2[0]  # single adjacent swap
    emit(n, s1, "".join(s2))
else:
    n = rng.randint(2, 26)
    a = alphabet(n)
    s1 = "".join(rng.sample(a, n))
    while True:
        if rng.random() < 0.5:
            s2 = "".join(rng.sample(a, n))
        else:
            k = rng.randint(1, n - 1)
            s2 = s1[k:] + s1[:k]
        if s2 != s1:
            break
    emit(n, s1, s2)
