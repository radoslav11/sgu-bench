"""Tests for p211 (Strange Counter): N <= 1000 registers, M <= 10000 adds
of 2^i, total sum below 2^N.

The generator tracks the remaining capacity 2^N - 1 - sum and only picks
exponents that fit. Seed 1 is the minimal 1-register single add. Seed 2
is N=1000, M=10000 biased to the highest exponent that still fits (long
carry chains). Seed 3 is N=1000, M=10000 all-ones (amortized carry
stress). Later seeds are random sizes and mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, adds):
    print(n)
    print(len(adds))
    print(" ".join(map(str, adds)))


if seed == 1:
    emit(1, [0])
    sys.exit()

if seed == 3:
    emit(1000, [0] * 10000)
    sys.exit()

if seed == 2:
    n, m = 1000, 10000
    style = "high"
elif seed == 4:
    n, m = 14, 10000
    style = "mixed"
else:
    n = rng.randint(1, 1000)
    m = rng.randint(1, 10000)
    style = rng.choice(["high", "low", "mixed"])

remaining = (1 << n) - 1
adds = []
for _ in range(m):
    if remaining < 1:
        break
    top = min(n - 1, remaining.bit_length() - 1)
    if style == "high":
        i = top if rng.random() < 0.7 else rng.randint(0, top)
    elif style == "low":
        i = rng.randint(0, min(3, top))
    else:
        i = rng.randint(0, top)
    adds.append(i)
    remaining -= 1 << i

emit(n, adds)
