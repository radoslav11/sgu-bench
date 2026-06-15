"""Tests for p148 (B-Station): N <= 15000 levels, 0 <= Wi, Li, Pi <= 15000.

Seed 1 is a single level. Seeds 2-3 are maximal N=15000: seed 2 fully
random weights, seed 3 mostly-zero W with rare heavy levels so cascades
run long before the water exceeds every L. Seed 4 is an all-zero-W test
(quadratic-cascade worst case, kept at N=4000 so the Python oracle
stays inside the build limit), seed 5 forces an empty optimal answer.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
    print(0, 0, 0)
    sys.exit()

if seed == 2:
    n = 15000
    rows = [
        (rng.randint(0, 15000), rng.randint(0, 15000), rng.randint(0, 15000))
        for _ in range(n)
    ]
elif seed == 3:
    n = 15000
    rows = []
    for _ in range(n):
        w = 8000 if rng.random() < 0.01 else 0
        rows.append((w, rng.randint(0, 15000), rng.randint(0, 15000)))
elif seed == 4:
    n = 4000
    rows = [(0, rng.randint(0, 15000), rng.randint(0, 15000)) for _ in range(n)]
elif seed == 5:
    n = 1
    rows = [(15000, 0, 15000)]  # bursts on its own, nothing to pay
else:
    n = rng.randint(1, 2000)
    style = rng.choice(["random", "small_w", "cheap", "heavy"])
    rows = []
    for _ in range(n):
        if style == "random":
            w = rng.randint(0, 15000)
        elif style == "small_w":
            w = rng.randint(0, 3)
        elif style == "cheap":
            w = rng.randint(0, 15000)
        else:
            w = 15000
        l = rng.randint(0, 15000)
        p = rng.randint(0, 5) if style == "cheap" else rng.randint(0, 15000)
        rows.append((w, l, p))

print(n)
print("\n".join(f"{w} {l} {p}" for w, l, p in rows))
