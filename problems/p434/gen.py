"""Tests for p434 (Chemists): N <= 21, 1 <= Si, Di <= 1000.

Seed 1 is the single-tube case. Seeds 2-3 are N = 21 with equal totals:
seed 2 uses generic values balanced via one adjusted entry (few zero-sum
subsets, the full bitmask DP runs), seed 3 is a shuffled permutation of
the same multiset (many zero-sum subsets). Later seeds mix small random
boards, -1 cases with different totals, and all-equal values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(s, d):
    print(len(s))
    print(*s)
    print(*d)


if seed == 1:
    emit([5], [5])
elif seed == 2:
    # Maximal n with equal totals but values chosen so zero-sum subsets
    # are rare: odd-ish spread values, then fix the total on one entry.
    n = 21
    while True:
        s = [rng.randint(1, 1000) for _ in range(n)]
        d = [rng.randint(1, 1000) for _ in range(n)]
        delta = sum(s) - sum(d)
        if 1 <= d[0] + delta <= 1000:
            d[0] += delta
            emit(s, d)
            break
elif seed == 3:
    # Maximal n, D is a shuffled copy of S: lots of zero-sum groups.
    n = 21
    s = [rng.randint(1, 1000) for _ in range(n)]
    d = s[:]
    rng.shuffle(d)
    emit(s, d)
elif seed == 4:
    # Maximal n, totals differ -> -1.
    n = 21
    s = [rng.randint(1, 1000) for _ in range(n)]
    d = [rng.randint(1, 1000) for _ in range(n)]
    if sum(s) == sum(d):
        d[0] += 1 if d[0] < 1000 else -1
    emit(s, d)
elif seed == 5:
    # Everything already in place: zero pourings.
    n = 21
    s = [rng.randint(1, 1000) for _ in range(n)]
    emit(s, s[:])
elif seed == 6:
    # All values equal.
    emit([7] * 21, [7] * 21)
else:
    n = rng.randint(1, 21)
    s = [rng.randint(1, 1000) for _ in range(n)]
    if rng.random() < 0.7:
        # Balanced totals built from a random partition of the sum.
        d = s[:]
        rng.shuffle(d)
        for _ in range(n * 2):
            i, j = rng.randrange(n), rng.randrange(n)
            move = rng.randint(0, min(d[i] - 1, 1000 - d[j]))
            d[i] -= move
            d[j] += move
        emit(s, d)
    else:
        d = [rng.randint(1, 1000) for _ in range(n)]
        emit(s, d)
