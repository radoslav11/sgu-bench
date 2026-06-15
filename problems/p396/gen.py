"""Tests for p396 (Dance it up!): song of N <= 1000 beats over L/U/R/D/N.

Seed 1 is the minimal song, seeds 2-3 are maximal (N = 1000) with
different letter distributions, later seeds mix adversarial rhythms:
alternating far buttons, long runs, rest-heavy songs, no rests at all.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

ALL = "LURD"

if seed == 1:
    print(1)
    print("L")
elif seed == 2:
    n = 1000
    print(n)
    print("".join(rng.choice(ALL + "N") for _ in range(n)))
elif seed == 3:
    # maximal song with no rests: every beat demands a button, with long
    # stretches alternating the two far buttons L and R
    n = 1000
    parts = []
    while len(parts) < n:
        block = rng.randint(1, 30)
        kind = rng.randrange(3)
        if kind == 0:
            parts.extend(("LR" * block)[:block])
        elif kind == 1:
            parts.extend(rng.choice(ALL) * block)
        else:
            parts.extend(rng.choice(ALL) for _ in range(block))
    print(n)
    print("".join(parts[:n]))
elif seed == 4:
    n = 1000
    print(n)
    print("N" * n)
elif seed == 5:
    n = 1000
    print(n)
    print(("UD" * n)[:n])
else:
    n = rng.randint(2, 1000)
    weights = [rng.random() for _ in range(5)]
    total = sum(weights)
    print(n)
    print(
        "".join(
            rng.choices(ALL + "N", weights=weights, k=1)[0] for _ in range(n)
        )
    )
