"""Tests for p155 (Cartesian Tree): N <= 50000, |k|, |a| <= 30000,
all main keys distinct, all auxiliary keys distinct (tree is unique).

Seed 1 is N=1. Seed 2 is N=50000 with random pairing. Seed 3 is N=50000
with a sorted the same way as k (the tree degenerates to a right path,
worst recursion depth). Seed 4 is the anti-sorted left path. The rest
are random mid sizes, with seed 7 a small fully random case and seed 8
a zig-zag pairing (alternating high/low priorities).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    n = 1
elif seed in (2, 3, 4):
    n = 50000
else:
    n = rng.randint(2, 20000)

ks = rng.sample(range(-30000, 30001), n)
avals = rng.sample(range(-30000, 30001), n)

if seed == 3:
    # a in the same order as k -> right path of depth n
    order = sorted(range(n), key=lambda i: ks[i])
    sa = sorted(avals)
    for rank, i in enumerate(order):
        avals[i] = sa[rank]
elif seed == 4:
    # a in opposite order of k -> left path of depth n
    order = sorted(range(n), key=lambda i: ks[i])
    sa = sorted(avals, reverse=True)
    for rank, i in enumerate(order):
        avals[i] = sa[rank]
elif seed == 8:
    # zig-zag: priorities alternate small/large along key order
    order = sorted(range(n), key=lambda i: ks[i])
    sa = sorted(avals)
    lo, hi = 0, n - 1
    for pos, i in enumerate(order):
        if pos % 2 == 0:
            avals[i] = sa[lo]
            lo += 1
        else:
            avals[i] = sa[hi]
            hi -= 1

print(n)
out = []
for i in range(n):
    out.append(f"{ks[i]} {avals[i]}")
print("\n".join(out))
