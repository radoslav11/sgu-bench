"""Tests for p269 (Rooks): n, k <= 250, row lengths b_i <= 250.

Seed 1 is the minimal 1x1 board. Seed 2 is the maximal full 250x250
board with k=250 (largest big-integer answer). Seed 3 is maximal with a
different shape: a random staircase of distinct widths. Other seeds mix
k > max(b) zero-answer cases, k = n boundaries, many equal rows, and
random boards.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, k, b):
    print(n, k)
    print(" ".join(map(str, b)))


if seed == 1:
    emit(1, 1, [1])
elif seed == 2:
    emit(250, 250, [250] * 250)
elif seed == 3:
    b = rng.sample(range(1, 251), 250)
    emit(250, 250, b)
elif seed == 4:
    # answer 0: more rooks than the widest row allows
    b = [rng.randint(1, 5) for _ in range(250)]
    emit(250, 100, b)
elif seed == 5:
    # k = 1: answer is just the number of cells
    b = [rng.randint(1, 250) for _ in range(250)]
    emit(250, 1, b)
else:
    n = rng.randint(1, 250)
    k = rng.randint(1, 250)
    style = rng.randrange(3)
    if style == 0:
        b = [rng.randint(1, 250) for _ in range(n)]
    elif style == 1:
        w = rng.randint(1, 250)
        b = [w] * n
    else:
        b = sorted(rng.randint(1, 250) for _ in range(n))[::-1]
    emit(n, k, b)
