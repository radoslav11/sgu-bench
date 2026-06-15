"""Tests for p429 (Problem Stacks): n heaps in a line, take-from-ends game.

1 <= n <= 5, 1 <= a_i <= 10^9. Seed 1 is the single-heap minimum, seed 2
is n=5 with every heap at the 10^9 maximum, seed 3 is another maximal n=5
test with a different (alternating) value pattern. Later seeds mix random
sizes with adversarial shapes: all equal, palindromic, and near-equal
ends that probe the P-position boundary.
"""

import random
import sys

MAX_A = 10**9

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(a):
    print(len(a))
    print(" ".join(map(str, a)))


if seed == 1:
    emit([1])
elif seed == 2:
    emit([MAX_A] * 5)
elif seed == 3:
    emit([MAX_A, 1, MAX_A, 1, MAX_A])
elif seed == 4:
    n = rng.randint(1, 5)
    v = rng.randint(1, MAX_A)
    emit([v] * n)
elif seed == 5:
    half = [rng.randint(1, MAX_A) for _ in range(3)]
    emit(half + half[1::-1])
elif seed == 6:
    n = 5
    base = rng.randint(1, MAX_A - 1)
    emit([base, rng.randint(1, MAX_A), rng.randint(1, MAX_A),
          rng.randint(1, MAX_A), base + rng.randint(0, 1)])
else:
    n = rng.randint(2, 5)
    if rng.random() < 0.4:
        hi = rng.randint(1, 12)
    else:
        hi = MAX_A
    emit([rng.randint(1, hi) for _ in range(n)])
