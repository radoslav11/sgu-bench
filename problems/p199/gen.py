"""Tests for p199 (Beautiful People): N members (2 <= N <= 100000) with
strength/beauty in [1, 10^9]; pick the largest antichain-free set, i.e.
the longest strictly-increasing-in-both-dimensions subset.

Seed 1 is minimal (N=2). Seed 2 is max-size fully random. Seed 3 is
max-size with values drawn from a tiny range (massive S/B ties, which
break naive LIS that forgets the equal-strength rule). Seed 4 is sorted
increasing (answer N), seed 5 anti-sorted (answer 1), seeds 6+ are random
mid/large sizes with mixed value ranges.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(pairs):
    print(len(pairs))
    print("\n".join(f"{s} {b}" for s, b in pairs))


if seed == 1:
    emit([(1, 1), (1000000000, 1000000000)])
elif seed == 2:
    emit(
        [
            (rng.randint(1, 10**9), rng.randint(1, 10**9))
            for _ in range(100000)
        ]
    )
elif seed == 3:
    emit([(rng.randint(1, 60), rng.randint(1, 60)) for _ in range(100000)])
elif seed == 4:
    n = 100000
    emit([(i + 1, 2 * i + 1) for i in range(n)])
elif seed == 5:
    n = 100000
    emit([(i + 1, n - i) for i in range(n)])
else:
    n = rng.randint(2, 100000)
    hi = rng.choice([10, 1000, 10**9])
    pairs = []
    for _ in range(n):
        if rng.random() < 0.3:
            # Clusters sharing one coordinate exercise the tie handling.
            base = rng.randint(1, hi)
            pairs.append(
                (base, rng.randint(1, hi))
                if rng.random() < 0.5
                else (rng.randint(1, hi), base)
            )
        else:
            pairs.append((rng.randint(1, hi), rng.randint(1, hi)))
    emit(pairs)
