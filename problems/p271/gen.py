"""Tests for p271 (Book Pile): N <= 40000 books, M <= 100000 operations.

Seed 1 is the empty pile with no operations. Seed 2 is maximal: full
pile, K = 40000, random ADD/ROTATE mix. Seed 3 is maximal with K = 2
(rotations only swap the top pair) and a ROTATE-heavy mix. Other seeds
cover K = 0 (rotations are no-ops), K larger than the pile ever gets,
all-ROTATE streams, N = 0 starts, and random parameters.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def name():
    return "".join(
        rng.choice(string.ascii_uppercase)
        for _ in range(rng.randint(1, 3))
    )


def emit(n, k, ops_count, add_prob):
    lines = []
    ops = []
    for _ in range(n):
        lines.append(name())
    for _ in range(ops_count):
        if rng.random() < add_prob:
            ops.append(f"ADD({name()})")
        else:
            ops.append("ROTATE")
    print(n, len(ops), k)
    sys.stdout.write("\n".join(lines + ops) + ("\n" if lines or ops else ""))


if seed == 1:
    print(0, 0, 0)
elif seed == 2:
    emit(40000, 40000, 100000, 0.5)
elif seed == 3:
    emit(40000, 2, 100000, 0.2)
elif seed == 4:
    emit(40000, 0, 100000, 0.5)  # K = 0: every ROTATE is a no-op
elif seed == 5:
    emit(0, 40000, 100000, 0.3)  # empty start, K above pile size for a while
elif seed == 6:
    emit(100, 40000, 100000, 0.0)  # rotations only, K always exceeds pile
else:
    n = rng.randint(0, 5000)
    k = rng.randint(0, 6000)
    m = rng.randint(0, 20000)
    emit(n, k, m, rng.random())
