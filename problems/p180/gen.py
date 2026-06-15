"""Tests for p180 (Inversions): count pairs i<j with A[i]>A[j].

1 <= N <= 65537, 0 <= Ai <= 10^9. Seed 1 is the minimal N=1 case,
seed 2 a strictly decreasing max-N array (maximum inversion count),
seed 3 a max-N uniform random array. Later seeds mix sorted, equal and
small-value-range arrays where ties matter.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAX_N = 65537
MAX_V = 10**9


def emit(arr):
    print(len(arr))
    print(" ".join(map(str, arr)))


if seed == 1:
    emit([rng.randint(0, MAX_V)])
elif seed == 2:
    emit(list(range(MAX_N, 0, -1)))
elif seed == 3:
    emit([rng.randint(0, MAX_V) for _ in range(MAX_N)])
elif seed == 4:
    # max N, tiny value range -> lots of duplicates
    emit([rng.randint(0, 3) for _ in range(MAX_N)])
elif seed == 5:
    # max N, already sorted (answer 0)
    arr = sorted(rng.randint(0, MAX_V) for _ in range(MAX_N))
    emit(arr)
elif seed == 6:
    # max N, all equal values
    emit([MAX_V] * MAX_N)
else:
    n = rng.randint(2, MAX_N)
    hi = rng.choice([1, 100, MAX_V])
    emit([rng.randint(0, hi) for _ in range(n)])
