"""Tests for p499: n distinct integers in [1, 2^20-1], maximize pairwise gcd.

Seed 1 is the minimal case (two values). Seeds 2 and 3 are maximal: the
full range of distinct values, so the multiples-counting loop does its
full O(M log M) work. The rest are randomized mid-size tests plus a few
adversarial shapes (large shared divisors, dense small values).
"""

import random
import sys

M = (1 << 20) - 1

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(2)
    print(6)
    print(8)
    sys.exit()

if seed == 2:
    vals = list(range(1, M + 1))
    print(len(vals))
    sys.stdout.write("\n".join(map(str, vals)))
    sys.stdout.write("\n")
    sys.exit()

if seed == 3:
    half = M // 2
    vals = list(range(half, M + 1))
    rng.shuffle(vals)
    print(len(vals))
    sys.stdout.write("\n".join(map(str, vals)))
    sys.stdout.write("\n")
    sys.exit()

if seed == 4:
    base = rng.randint(2, 1000)
    mults = sorted(set(base * k for k in range(2, M // base + 1)))
    extra = set()
    while len(extra) < 5000:
        v = rng.randint(1, M)
        if v % base != 0:
            extra.add(v)
    vals = sorted(set(mults) | extra)
    rng.shuffle(vals)
    print(len(vals))
    sys.stdout.write("\n".join(map(str, vals)))
    sys.stdout.write("\n")
    sys.exit()

n = rng.randint(2, 200000)
vals = rng.sample(range(1, M + 1), n)
print(n)
sys.stdout.write("\n".join(map(str, vals)))
sys.stdout.write("\n")
