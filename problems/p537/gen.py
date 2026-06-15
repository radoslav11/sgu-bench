"""Tests for p537 (Divisibility): n <= 100 patterns, each 1..14 lowercase
letters with at most 10 distinct characters.

Output is the set of universal divisors per pattern, so the work is fixed
by n and pattern length regardless of values; the maximal seeds simply use
n = 100 patterns of length 14 at the distinct-letter cap. Seed 1 is the
minimal input (one single-letter pattern). Seeds 2 and 3 are maximal with
different letter structure (all-same-letter vs. exactly 10 distinct).
Later seeds randomize n, lengths, and alphabet size, including the
repeated-block patterns (like "lalala") that yield rich divisor sets.
"""

import random
import sys

MAXN = 100
MAXLEN = 14
MAXDISTINCT = 10
ALPHABET = "abcdefghijklmnopqrstuvwxyz"


def random_pattern(rng, length=None, distinct=None):
    if length is None:
        length = rng.randint(1, MAXLEN)
    if distinct is None:
        distinct = rng.randint(1, min(MAXDISTINCT, length))
    letters = rng.sample(ALPHABET, distinct)
    # Guarantee every chosen letter appears so the distinct count is exact.
    base = list(letters)
    base += [rng.choice(letters) for _ in range(length - distinct)]
    rng.shuffle(base)
    return "".join(base[:length])


def emit(patterns):
    print(len(patterns))
    print("\n".join(patterns))


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(["a"])
    sys.exit()

if seed == 2:
    # Maximal n, all patterns a single repeated letter (max divisor sets).
    emit(["a" * MAXLEN for _ in range(MAXN)])
    sys.exit()

if seed == 3:
    # Maximal n, every pattern uses exactly 10 distinct letters.
    emit([random_pattern(rng, MAXLEN, MAXDISTINCT) for _ in range(MAXN)])
    sys.exit()

if seed == 4:
    # Repeated-block patterns of varied period, where universal divisors are
    # most interesting (e.g. "lalala", "icpcicpc").
    patterns = []
    for _ in range(MAXN):
        period = rng.randint(1, 4)
        block = random_pattern(rng, period, min(period, MAXDISTINCT))
        reps = rng.randint(1, MAXLEN // period)
        patterns.append((block * reps)[:MAXLEN])
    emit(patterns)
    sys.exit()

n = rng.randint(1, MAXN)
emit([random_pattern(rng) for _ in range(n)])
