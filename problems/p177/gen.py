"""Tests for p177 (Square): repaint rectangles, count white cells.

N <= 1000, M <= 5000, corners of each rectangle may come in any order.
Seed 1 is the 1x1 minimum, seed 2 the maximum with huge rectangles
(worst case for per-row work), seed 3 the maximum with tiny scattered
rectangles and skewed colors. Later seeds cover the sample, thin
1-wide stripes, and random mid sizes with swapped corners.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, ops):
    print(n, len(ops))
    for x1, y1, x2, y2, c in ops:
        print(x1, y1, x2, y2, c)


def rect(rng, n, max_side=None):
    if max_side is None:
        max_side = n
    x1 = rng.randint(1, n)
    y1 = rng.randint(1, n)
    x2 = min(n, x1 + rng.randint(0, max_side - 1))
    y2 = min(n, y1 + rng.randint(0, max_side - 1))
    if rng.random() < 0.5:
        x1, x2 = x2, x1
    if rng.random() < 0.5:
        y1, y2 = y2, y1
    return x1, y1, x2, y2


if seed == 1:
    emit(1, [(1, 1, 1, 1, 'b')])
elif seed == 2:
    # maximal: N = 1000, M = 5000, rectangles spanning most of the square
    n = 1000
    ops = []
    for _ in range(5000):
        x1 = rng.randint(1, 30)
        y1 = rng.randint(1, 30)
        x2 = rng.randint(n - 29, n)
        y2 = rng.randint(n - 29, n)
        if rng.random() < 0.5:
            x1, x2 = x2, x1
        if rng.random() < 0.5:
            y1, y2 = y2, y1
        ops.append((x1, y1, x2, y2, rng.choice('bw')))
    emit(n, ops)
elif seed == 3:
    # maximal size, tiny scattered rectangles, mostly black
    n = 1000
    ops = [
        rect(rng, n, 12) + (('b' if rng.random() < 0.8 else 'w'),)
        for _ in range(5000)
    ]
    emit(n, ops)
elif seed == 4:
    emit(9, [
        (2, 2, 4, 6, 'b'), (4, 3, 3, 3, 'w'), (6, 2, 8, 6, 'b'),
        (5, 3, 6, 9, 'w'), (8, 3, 9, 9, 'w'), (1, 5, 3, 5, 'w'),
    ])
elif seed == 5:
    # thin stripes: 1-wide columns and rows over the full square
    n = 1000
    ops = []
    for _ in range(5000):
        k = rng.randint(1, n)
        if rng.random() < 0.5:
            ops.append((k, 1, k, n, rng.choice('bw')))
        else:
            ops.append((1, k, n, k, rng.choice('bw')))
    emit(n, ops)
elif seed == 6:
    # paint everything black, then everything white, repeatedly
    n = 1000
    ops = [(1, 1, n, n, 'b' if i % 2 == 0 else 'w') for i in range(4999)]
    ops.append(rect(rng, n) + ('b',))
    emit(n, ops)
else:
    n = rng.randint(2, 1000)
    m = rng.randint(1, 5000)
    ops = [rect(rng, n) + (rng.choice('bw'),) for _ in range(m)]
    emit(n, ops)
