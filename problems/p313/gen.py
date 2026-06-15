"""Tests for p313 (Circular Railway): n houses and n offices on a circle.

1 <= n <= 50000, 2 <= L <= 10^9. Seed 1 is minimal (n=1, L=2). Seed 2 is
maximal with uniform random stations on a huge circle; seed 3 is maximal
with houses packed in one arc and offices in the opposite arc (matching
must wrap both ways around the circle). Other seeds cover tiny L with
massive duplication, all points at one station, and mid-size randoms.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, length, houses, offices):
    print(n, length)
    print(" ".join(map(str, houses)))
    print(" ".join(map(str, offices)))


if seed == 1:
    emit(1, 2, [1], [2])
elif seed == 2:
    n, length = 50000, 10**9
    emit(
        n,
        length,
        [rng.randint(1, length) for _ in range(n)],
        [rng.randint(1, length) for _ in range(n)],
    )
elif seed == 3:
    n, length = 50000, 10**9
    houses = [rng.randint(1, length // 20) for _ in range(n)]
    offices = [rng.randint(length // 2, length // 2 + length // 20) for _ in range(n)]
    emit(n, length, houses, offices)
elif seed == 4:
    n, length = 50000, 2
    emit(
        n,
        length,
        [rng.randint(1, 2) for _ in range(n)],
        [rng.randint(1, 2) for _ in range(n)],
    )
elif seed == 5:
    n, length = 50000, 100
    emit(
        n,
        length,
        [rng.randint(1, 100) for _ in range(n)],
        [rng.randint(1, 100) for _ in range(n)],
    )
elif seed == 6:
    n, length = 30000, 10**9
    emit(n, length, [7] * n, [length // 2] * n)
else:
    n = rng.randint(1, 3000)
    length = rng.choice([2, 17, 1000, 10**6, 10**9])
    emit(
        n,
        length,
        [rng.randint(1, length) for _ in range(n)],
        [rng.randint(1, length) for _ in range(n)],
    )
