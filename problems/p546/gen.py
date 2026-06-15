"""Tests for p546 (Ternary Password).

Input: line 1 "n a b" (1<=n<=200, 0<=a,b<=200); line 2 a ternary password
of length n over {0,1,2}.

Seed 1 is the minimal n=1 case. Seeds 2-3 are maximal n=200 with two
different a,b and password distributions. Later seeds cover already-valid
passwords, the infeasible case a+b>n, swap-heavy cases (lots of surplus 0s
needed as 1s and vice versa), and random mid-size inputs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, a, b, s):
    print(n, a, b)
    print(s)


if seed == 1:
    emit(1, 0, 1, "2")
    sys.exit()

if seed == 2:
    n = 200
    a, b = 100, 100
    # all twos: requires the maximum number of replacements
    emit(n, a, b, "2" * n)
    sys.exit()

if seed == 3:
    n = 200
    a, b = 90, 70
    # password heavy on the "wrong" digits to force many swaps/conversions
    s = "".join(rng.choice("01") for _ in range(n))
    emit(n, a, b, s)
    sys.exit()

if seed == 4:
    # already valid: build a password then ask for exactly its counts
    n = 200
    s = "".join(rng.choice("012") for _ in range(n))
    emit(n, s.count("0"), s.count("1"), s)
    sys.exit()

if seed == 5:
    # infeasible: a + b > n
    n = 200
    emit(n, 150, 150, "".join(rng.choice("012") for _ in range(n)))
    sys.exit()

if seed == 6:
    # swap-heavy: many 0s present but ones required, and vice versa
    n = 200
    s = "0" * 100 + "1" * 100
    s = "".join(rng.sample(s, len(s)))
    emit(n, 100, 100, s)
    sys.exit()

n = rng.randint(1, 200)
a = rng.randint(0, 200)
b = rng.randint(0, 200)
s = "".join(rng.choice("012") for _ in range(n))
emit(n, a, b, s)
