"""Tests for p146 (The Runner): ring length L in [1, 1000] with 4
decimals, N <= 20000 intervals, Ti <= 1e7, Vi <= 1e6.

Seed 1 is the smallest case (L=1, one interval). Seeds 2-3 are maximal
N=20000 with extreme Ti*Vi products (int64-overflow bait): seed 2 all
values at their caps, seed 3 random extremes with fractional L. Later
seeds randomize everything, including L values that make the runner
finish very close to the start.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print("1.0000 1")
    print(1, 1)
    sys.exit()

if seed == 2:
    l_scaled = 10000000  # L = 1000.0000
    n = 20000
    pairs = [(10**7, 10**6)] * n
elif seed == 3:
    l_scaled = rng.randint(10000, 10000000)
    n = 20000
    pairs = [
        (rng.randint(9 * 10**6, 10**7), rng.randint(9 * 10**5, 10**6))
        for _ in range(n)
    ]
elif seed == 4:
    # distance lands exactly on half the ring: both arcs equal
    l_scaled = 20000  # L = 2.0000
    n = 1
    pairs = [(1, 1)]
else:
    l_scaled = rng.randint(10000, 10000000)
    n = rng.randint(1, 20000)
    pairs = [
        (rng.randint(1, 10**7), rng.randint(1, 10**6)) for _ in range(n)
    ]

print(f"{l_scaled / 10000:.4f} {n}")
out = [f"{t} {v}" for t, v in pairs]
print("\n".join(out))
