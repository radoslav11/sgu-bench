"""Tests for p292 (Field for the Cemetery): three lines q, c, n with
0 <= q, c <= 10^1000 and 1 <= n <= 10^1000.

Seed 1 is the minimal input (0, 0, 1). Seed 2 is maximal: q = c =
10^1000 with n = 1, the largest possible numbers and output. Seed 3 is a
second max-size test with 1001/1000-digit random values where both sides
exceed n, exercising the pinwheel bonus. Later seeds cover one side
shorter than n, both sides shorter, exact divisibility, the s + t = n
boundary, and random digit lengths.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def rand_digits(num_digits):
    first = rng.randint(1, 9)
    rest = [rng.randint(0, 9) for _ in range(num_digits - 1)]
    return int("".join(map(str, [first] + rest)))


if seed == 1:
    q, c, n = 0, 0, 1
elif seed == 2:
    q, c, n = 10**1000, 10**1000, 1
elif seed == 3:
    q = rng.randint(10**999, 10**1000)
    c = rng.randint(10**999, 10**1000)
    n = rand_digits(999)
elif seed == 4:
    # one side shorter than n
    n = rand_digits(500)
    q = rng.randint(0, n - 1)
    c = rand_digits(1000)
elif seed == 5:
    # both sides shorter than n -> 0
    n = rand_digits(900)
    q = rng.randint(0, n - 1)
    c = rng.randint(0, n - 1)
elif seed == 6:
    # n divides both sides exactly
    n = rand_digits(400)
    q = n * rand_digits(300)
    c = n * rand_digits(250)
elif seed == 7:
    # remainders with s + t exactly n (no bonus) on huge numbers
    n = rand_digits(600)
    s = rng.randint(1, n - 1)
    t = n - s
    q = n * rand_digits(300) + s
    c = n * rand_digits(300) + t
elif seed == 8:
    # s + t = n + 1, smallest possible bonus
    n = rand_digits(600)
    s = rng.randint(2, n - 1)
    t = n + 1 - s
    q = n * rand_digits(300) + s
    c = n * rand_digits(300) + t
else:
    q = rand_digits(rng.randint(1, 1000))
    c = rand_digits(rng.randint(1, 1000))
    n = rand_digits(rng.randint(1, 1000))

print(q)
print(c)
print(n)
