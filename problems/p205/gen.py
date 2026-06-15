"""Tests for p205 (Quantization Problem).

n <= 1000 values in [1, 1e6], m level sets of s levels each (powers of two,
m <= s <= 128, levels increasing). Seed 1 is the minimal n=m=s=1 case,
seeds 2-3 are maximal n=1000, m=s=128 (uniform vs clustered levels with
anti-greedy x values), later seeds vary m, s and value distributions.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10**6


def levels_uniform(s):
    return sorted(rng.sample(range(1, MAXV + 1), s))


def levels_clustered(s):
    center = rng.randint(1, MAXV)
    vals = set()
    while len(vals) < s:
        vals.add(max(1, min(MAXV, center + rng.randint(-200, 200))))
    return sorted(vals)


def emit(n, xs, m, s, sets):
    print(n)
    print(" ".join(map(str, xs)))
    print(m, s)
    for row in sets:
        print(" ".join(map(str, row)))


if seed == 1:
    emit(1, [1], 1, 1, [[MAXV]])
    sys.exit()

if seed == 2:
    n, m, s = 1000, 128, 128
    xs = [rng.randint(1, MAXV) for _ in range(n)]
    sets = [levels_uniform(s) for _ in range(m)]
elif seed == 3:
    n, m, s = 1000, 128, 128
    sets = [levels_clustered(s) for _ in range(m)]
    centers = [row[s // 2] for row in sets]
    xs = [
        max(1, min(MAXV, rng.choice(centers) + rng.randint(-500, 500)))
        for _ in range(n)
    ]
else:
    n = rng.randint(1, 1000)
    m = 2 ** rng.randint(0, 7)
    s = 2 ** rng.randint(max(0, m.bit_length() - 1), 7)
    maker = rng.choice([levels_uniform, levels_clustered])
    sets = [maker(s) for _ in range(m)]
    if seed % 2 == 0:
        xs = [rng.randint(1, MAXV) for _ in range(n)]
    else:
        pool = [v for row in sets for v in row]
        xs = [
            max(1, min(MAXV, rng.choice(pool) + rng.randint(-3, 3)))
            for _ in range(n)
        ]

emit(n, xs, m, s, sets)
