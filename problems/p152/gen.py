"""Tests for p152 (round vote percentages to integers summing to 100).

N <= 10000 candidates, 0 <= Ai <= 10000 votes. The vote total is always
kept positive (an all-zero vote vector would make the percentages
undefined). Adversarial patterns: all-equal votes (every part 0.01%),
many zeros, two-candidate near-ties, and inputs with multiple spaces
between numbers as the statement explicitly allows.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
    print(10000)
elif seed == 2:
    # maximal: N = 10000, all votes at the cap
    print(10000)
    print(" ".join(["10000"] * 10000))
elif seed == 3:
    # maximal with different structure: every share is exactly 0.01%
    print(10000)
    print(" ".join(["1"] * 10000))
elif seed == 4:
    # mostly zeros, a few positive entries
    n = 10000
    a = [0] * n
    for i in rng.sample(range(n), 7):
        a[i] = rng.randint(1, 10000)
    print(n)
    print(" ".join(map(str, a)))
elif seed == 5:
    print(2)
    print("10 10")
elif seed == 6:
    # near-tie: 1/3, 1/3, 1/3 style fractions
    print(3)
    print("3333 3333 3334")
else:
    n = rng.randint(2, 10000)
    if seed % 2 == 0:
        a = [rng.randint(0, 10000) for _ in range(n)]
    else:
        a = [rng.choice([0, 0, 1, 2, 3, rng.randint(0, 10000)]) for _ in range(n)]
    if sum(a) == 0:
        a[rng.randrange(n)] = rng.randint(1, 10000)
    sep_pool = [" ", "  ", "   "]
    parts = [str(a[0])]
    for x in a[1:]:
        parts.append(rng.choice(sep_pool))
        parts.append(str(x))
    print(n)
    print("".join(parts))
