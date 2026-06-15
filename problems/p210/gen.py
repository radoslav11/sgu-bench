"""Tests for p210 (Beloved Sons): N <= 400 sons, loves A_i in [1, 1000],
each son lists the girls (1..N) he likes.

Seed 1 is the minimal single-son case. Seed 2 is N=400 with dense random
preference lists. Seed 3 is N=400 adversarial: all loves equal and
preferences confined to narrow windows, forcing long augmenting paths.
Later seeds are random sizes with sparse/dense lists and tied loves.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(loves, prefs):
    n = len(loves)
    print(n)
    print(" ".join(map(str, loves)))
    for p in prefs:
        print(len(p), " ".join(map(str, p)))


if seed == 1:
    emit([1000], [[1]])
    sys.exit()

if seed == 2:
    n = 400
    loves = [rng.randint(1, 1000) for _ in range(n)]
    prefs = [
        rng.sample(range(1, n + 1), rng.randint(100, 250)) for _ in range(n)
    ]
elif seed == 3:
    n = 400
    loves = [500] * n
    prefs = []
    for i in range(n):
        lo = max(1, i - 3)
        hi = min(n, i + 3)
        prefs.append(rng.sample(range(lo, hi + 1), rng.randint(1, hi - lo + 1)))
else:
    n = rng.randint(1, 400)
    if seed % 2 == 0:
        loves = [rng.randint(1, 1000) for _ in range(n)]
    else:
        loves = [rng.choice([1, 7, 7, 1000]) for _ in range(n)]
    prefs = []
    for i in range(n):
        k = rng.randint(1, min(n, rng.choice([1, 2, 3, 8, 40])))
        prefs.append(rng.sample(range(1, n + 1), k))

emit(loves, prefs)
