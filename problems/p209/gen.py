"""Tests for p209 (Areas): N <= 80 distinct lines, integer coords |.| <= 100.

Lines are deduplicated by their normalized (a, b, c) equation. Seed 1 is
a single line (K = 0), seed 2 is 80 random lines, seed 3 is 80 grid +
diagonal lines (many parallels and concurrent intersections, many cells).
Later seeds mix small coordinate boxes, axis-parallel bundles and fans
through a common point.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

C = 100


def norm(x1, y1, x2, y2):
    a = y2 - y1
    b = x1 - x2
    c = a * x1 + b * y1
    g = math.gcd(math.gcd(abs(a), abs(b)), abs(c)) or 1
    a, b, c = a // g, b // g, c // g
    if (a, b, c) < (0, 0, 0) or (a < 0) or (a == 0 and b < 0):
        a, b, c = -a, -b, -c
    return (a, b, c)


def emit(lines):
    print(len(lines))
    for x1, y1, x2, y2 in lines:
        print(x1, y1, x2, y2)


def add(lines, used, cand):
    x1, y1, x2, y2 = cand
    if (x1, y1) == (x2, y2):
        return
    key = norm(x1, y1, x2, y2)
    if key in used:
        return
    used.add(key)
    lines.append(cand)


if seed == 1:
    emit([(0, 0, 1, 0)])
    sys.exit()

lines, used = [], set()

if seed == 2:
    while len(lines) < 80:
        add(lines, used, tuple(rng.randint(-C, C) for _ in range(4)))
elif seed == 3:
    for i in range(-20, 20, 2):
        add(lines, used, (i, -C, i, C))
        add(lines, used, (-C, i, C, i))
    while len(lines) < 80:
        p = rng.randint(-30, 30)
        q = rng.randint(-30, 30)
        add(lines, used, (p, q, p + rng.randint(1, 5), q + rng.randint(-5, 5)))
else:
    n_target = rng.randint(2, 80)
    style = seed % 4
    while len(lines) < n_target:
        if style == 0:
            box = rng.choice([3, 10, 100])
            add(lines, used, tuple(rng.randint(-box, box) for _ in range(4)))
        elif style == 1:
            x1, y1 = rng.randint(-C, C), rng.randint(-C, C)
            dx, dy = rng.choice([(1, 0), (0, 1), (1, 1), (1, -1), (2, 1)])
            add(lines, used, (x1, y1, x1 + dx, y1 + dy))
        elif style == 2:
            px, py = 7, -3
            x2, y2 = rng.randint(-C, C), rng.randint(-C, C)
            add(lines, used, (px, py, x2, y2))
        else:
            add(lines, used, tuple(rng.randint(-C, C) for _ in range(4)))

emit(lines)
