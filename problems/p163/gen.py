"""Tests for p163 (Wise King): pick sons maximizing sum of p-th powers.

Input: N <= 100 sons on line 1, exponent 1 <= p <= 3 on line 2, then N
potentials in [-3, 3] on line 3. Seed 1 is the minimal single-son case,
seeds 2-3 are maximal (N = 100) with different exponents and value
distributions. Later seeds cover all-negative lists (answer 0 unless p
is even), all zeros, and random mixes for every exponent.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(p, values):
    print(len(values))
    print(p)
    print(*values)


if seed == 1:
    emit(1, [-1])
elif seed == 2:
    # maximal size, cubes, full mix of values including ties at 0
    emit(3, [rng.randint(-3, 3) for _ in range(100)])
elif seed == 3:
    # maximal size, squares: negative potentials still contribute
    emit(2, [rng.choice([-3, -2, -1]) for _ in range(100)])
elif seed == 4:
    # all negative with odd exponent -> answer must be 0
    emit(3, [rng.randint(-3, -1) for _ in range(100)])
elif seed == 5:
    # all zeros
    emit(rng.randint(1, 3), [0] * 100)
elif seed == 6:
    emit(1, [rng.randint(-3, 3) for _ in range(100)])
else:
    n = rng.randint(1, 100)
    emit(rng.randint(1, 3), [rng.randint(-3, 3) for _ in range(n)])
