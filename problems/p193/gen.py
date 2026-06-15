"""Tests for p193 (Chinese Girls' Amusement): one integer N with
3 <= N <= 10^2000.

Seed 1 is N = 3. Seed 2 is a random 2000-digit odd N, seed 3 is exactly
10^2000 (the largest allowed value, divisible by 4). Seeds 4-5 are
2000-digit numbers congruent to 2 and 0 mod 4. Seeds 6+ cover small N
(all residues mod 4), powers of two and random digit counts.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def big(digits, mod4=None):
    n = rng.randint(10 ** (digits - 1), 10**digits - 1)
    if mod4 is not None:
        n += (mod4 - n) % 4
    return n


if seed == 1:
    print(3)
elif seed == 2:
    print(big(2000, 3))
elif seed == 3:
    print(10**2000)
elif seed == 4:
    print(big(2000, 2))
elif seed == 5:
    print(big(2000, 0))
elif seed == 6:
    print(rng.choice([4, 5, 6, 7, 8, 9, 10, 12]))
elif seed == 7:
    print(2 ** rng.randint(700, 6643))  # huge power of two, still <= 10^2000
else:
    print(min(big(rng.randint(1, 2000)) + 3, 10**2000))
