"""Tests for p299 (Triangle): 3 <= N <= 1000, lengths in [1, 10^500].

Seed 1 is minimal. Seed 2 is maximal with random 500-digit lengths
(triangle exists). Seed 3 is maximal with a Fibonacci sequence (no
triangle, forces big-integer comparisons through the whole scan).
Seed 4 plants a single valid triple inside a Fibonacci sequence,
seed 5 uses powers of two (no triangle), the rest are random with
many duplicates and mixed magnitudes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIMIT = 10**500


def emit(lengths):
    assert 3 <= len(lengths) <= 1000
    print(len(lengths))
    for v in lengths:
        assert 1 <= v <= LIMIT
        print(v)


def rand_big(digits):
    lo = 10 ** (digits - 1)
    return rng.randint(lo, min(10**digits - 1, LIMIT))


fib = [1, 2]
while len(fib) < 1000:
    fib.append(fib[-1] + fib[-2])

if seed == 1:
    emit([1, 1, 1])
elif seed == 2:
    emit([rand_big(500) for _ in range(1000)])
elif seed == 3:
    emit(fib[:1000])  # no triangle, ~209-digit values at the tail
elif seed == 4:
    vals = fib[:999] + [fib[700]]  # duplicate creates a triangle
    rng.shuffle(vals)
    emit(vals)
elif seed == 5:
    emit([2**k for k in range(1, 1001)])  # no triangle
elif seed == 6:
    emit([LIMIT] * 1000)  # all equal at the upper bound
else:
    n = rng.randint(3, 1000)
    vals = []
    while len(vals) < n:
        v = rand_big(rng.randint(1, 500))
        vals.append(v)
        if rng.random() < 0.3 and len(vals) < n:
            vals.append(v)  # duplicates
    rng.shuffle(vals)
    emit(vals[:n])
