"""Tests for p492 (Hotel in Ves Lagos): T<=100, 1<=N<=1e18.

Seed 1 is a single minimal query. Seeds 2-3 are maximal (T=100): one with
every query at the upper bound 1e18, one with large random queries. Later
seeds mix small values, powers of ten, and values straddling boundaries
where '13' first appears.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXN = 10 ** 18


def emit(vals):
    print(len(vals))
    for v in vals:
        print(v)


if seed == 1:
    emit([1])
elif seed == 2:
    emit([MAXN] * 100)
elif seed == 3:
    emit([rng.randint(10 ** 17, MAXN) for _ in range(100)])
elif seed == 4:
    emit(list(range(1, 31)) + [10 ** k for k in range(0, 19)])
elif seed == 5:
    vals = []
    for _ in range(100):
        e = rng.randint(0, 18)
        vals.append(rng.randint(1, min(MAXN, 10 ** (e + 1))))
    emit(vals)
elif seed == 6:
    # Values around small numbers where 13, 130-139, 1300.. get skipped.
    emit([12, 13, 14, 119, 120, 121, 1287, 1288, 1289, 1290])
else:
    cnt = rng.randint(1, 100)
    emit([rng.randint(1, MAXN) for _ in range(cnt)])
