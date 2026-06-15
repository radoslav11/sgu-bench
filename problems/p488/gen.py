"""Tests for p488 (Dales and Hills): T sequences, find the highest hill and
deepest dale (a hill strictly rises then strictly falls; depth/height is the
min of the two run lengths).

The reconstructed limits are T <= 10000, N >= 1, the sum of N over all test
cases <= 1_000_000, and |a_i| <= 1e9. Each test is T, then for every case a
line with N and a line with the N values. Seed 1 is the minimal single
short case, seeds 2-3 are the maximal-total cases (one giant sequence, and
the maximum number of cases), and later seeds use monotone runs (one huge
hill/dale), plateaus, tight zigzags, and all-equal sequences.
"""

import random
import sys

SUM_N = 1_000_000
MAX_T = 10_000
MAX_A = 10**9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(cases):
    out = [str(len(cases))]
    for seq in cases:
        out.append(str(len(seq)))
        out.append(" ".join(str(x) for x in seq))

    sys.stdout.write("\n".join(out) + "\n")


def rand_seq(n, lo=-MAX_A, hi=MAX_A):
    return [rng.randint(lo, hi) for _ in range(n)]


if seed == 1:
    emit([[rng.randint(-10, 10)]])
elif seed == 2:
    # Single maximal sequence using the whole N budget, random values.
    emit([rand_seq(SUM_N)])
elif seed == 3:
    # Maximum number of test cases sharing the N budget evenly.
    t = MAX_T
    per = SUM_N // t
    emit([rand_seq(per) for _ in range(t)])
elif seed == 4:
    # One huge strictly increasing then strictly decreasing run: the single
    # tallest possible hill, plus the matching deepest dale at the edges.
    half = SUM_N // 2
    seq = list(range(half)) + list(range(half, -1, -1))
    seq = seq[:SUM_N]
    emit([seq])
elif seed == 5:
    # Tight zigzag: every interior index is both a peak and a valley of
    # height/depth 1.
    seq = [(i % 2) for i in range(SUM_N)]
    emit([seq])
elif seed == 6:
    # Plateaus everywhere: equal neighbours break all runs, so no hill or
    # dale of positive size from the flat parts.
    block = rng.randint(2, 5)
    seq = []
    val = 0
    while len(seq) < SUM_N:
        seq.extend([val] * block)
        val += rng.choice([-1, 1])

    emit([seq[:SUM_N]])
elif seed == 7:
    # Many medium cases with a small value range to create lots of ties.
    t = 2000
    per = SUM_N // t
    emit([[rng.randint(-3, 3) for _ in range(per)] for _ in range(t)])
elif seed == 8:
    # All equal: no hills, no dales.
    emit([[rng.randint(-MAX_A, MAX_A)] * SUM_N])
elif seed == 9:
    # Sawtooth with a fixed period to create many equal-height hills.
    period = 7
    seq = []
    for i in range(SUM_N):
        p = i % (2 * period)
        seq.append(p if p <= period else 2 * period - p)

    emit([seq])
else:
    cases = []
    total = 0
    while total < SUM_N and len(cases) < MAX_T:
        remaining = SUM_N - total
        n = rng.randint(1, min(remaining, 5000))
        cases.append(rand_seq(n, -100, 100))
        total += n

    emit(cases)
