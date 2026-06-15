"""Tests for p480 (Gena's Soul Cakes): c colors, k size indices (both
<= 1000), availability a_ij <= 1000.

Seed 1 is minimal (c = k = 1, exactly 8 unit corners). Seed 2 is the
maximal fully random instance. Seed 3 is maximal with all volume just
below the top sizes spread thinly over colors, so many colors are
needed. Later seeds: maximal infeasible (all mass at small sizes),
exact-volume boundary, equal-volume ties, random mid sizes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(c, k, a):
    print(c, k)
    for row in a:
        print(*row)


if seed == 1:
    emit(1, 1, [[8]])
elif seed == 2:
    c, k = 1000, 1000
    emit(c, k, [[rng.randint(0, 1000) for _ in range(k)] for _ in range(c)])
elif seed == 3:
    # Volume concentrated at size k-2, one corner per color: the target
    # 8^k = 64 * 8^(k-2) needs 64 colors; low sizes hold worthless dust.
    c, k = 1000, 1000
    a = [[0] * k for _ in range(c)]
    for i in range(c):
        a[i][k - 2] = 1
        for _ in range(20):
            a[i][rng.randrange(0, k // 2)] = rng.randint(0, 1000)
    emit(c, k, a)
elif seed == 4:
    # All mass below size k/2: maximum possible volume is far under 8^k.
    c, k = 1000, 1000
    a = [
        [rng.randint(0, 1000) if j < k // 2 else 0 for j in range(k)]
        for i in range(c)
    ]
    emit(c, k, a)
elif seed == 5:
    # One color holding exactly 8^k in mixed sizes, plus zero-volume colors.
    # Exact construction: repeatedly split the big corner, keeping a random
    # share at each size, so one color holds exactly 8^k in mixed sizes.
    k = rng.randint(2, 1000)
    row = [0] * k
    remaining = 1  # corners of current size, starting at size k
    size = k
    while size > 0:
        remaining *= 8  # split all into size-1 smaller
        size -= 1
        if size == 0:
            row[0] += remaining
            break
        keep = rng.randint(0, min(remaining, 900))
        if remaining - keep > 100:  # keep the split chain short
            keep = remaining - rng.randint(1, 100)
        row[size] += keep
        remaining -= keep
    c = 50
    a = [[0] * k for _ in range(c)]
    a[rng.randrange(c)] = row
    emit(c, k, a)
elif seed == 6:
    # Many colors with identical volume; minimum is a large prefix.
    c, k = 500, 500
    a = [[0] * k for _ in range(c)]
    for i in range(c):
        a[i][k - 3] = 4
    emit(c, k, a)
else:
    c = rng.randint(1, 60)
    k = rng.randint(1, 60)
    density = rng.choice([3, 20, 1000])
    emit(
        c,
        k,
        [[rng.randint(0, density) for _ in range(k)] for _ in range(c)],
    )
