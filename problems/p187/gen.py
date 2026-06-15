"""Tests for p187 (Twist and whirl): M segment reversals of 1..N.

1 <= N <= 130000, 1 <= M <= 2000, 1 <= Pi <= Qi <= N. Seed 1 is the
minimal N=1 case, seed 2 has max N and M with huge random segments,
seed 3 max N and M with full-array and prefix/suffix reversals. Other
seeds use random N/M, tiny segments and repeated identical reversals.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_N = 130000
MAX_M = 2000


def emit(n, ops):
    print(n, len(ops))
    print("\n".join(f"{p} {q}" for p, q in ops))


if seed == 1:
    emit(1, [(1, 1)])
elif seed == 2:
    ops = []
    for _ in range(MAX_M):
        p = rng.randint(1, MAX_N)
        q = rng.randint(p, MAX_N)
        ops.append((p, q))
    emit(MAX_N, ops)
elif seed == 3:
    # alternating full reversals and random prefixes/suffixes
    ops = []
    for i in range(MAX_M):
        if i % 3 == 0:
            ops.append((1, MAX_N))
        elif i % 3 == 1:
            ops.append((1, rng.randint(1, MAX_N)))
        else:
            ops.append((rng.randint(1, MAX_N), MAX_N))
    emit(MAX_N, ops)
elif seed == 4:
    # max sizes, single-element and length-2 reversals
    ops = []
    for _ in range(MAX_M):
        p = rng.randint(1, MAX_N - 1)
        ops.append((p, p + rng.randint(0, 1)))
    emit(MAX_N, ops)
elif seed == 5:
    # the same middle segment reversed over and over (odd count)
    emit(MAX_N, [(MAX_N // 4, 3 * MAX_N // 4)] * (MAX_M - 1))
else:
    n = rng.randint(1, MAX_N)
    m = rng.randint(1, MAX_M)
    ops = []
    for _ in range(m):
        p = rng.randint(1, n)
        q = rng.randint(p, n)
        ops.append((p, q))
    emit(n, ops)
