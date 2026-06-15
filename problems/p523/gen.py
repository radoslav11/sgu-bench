"""Tests for p523 (Elevator): n,f <= 100, buttons distinct in [1,100],
all != f. Since values come from 1..100 minus f, the maximum n is 99.

Seed 1 is a single button press. Seeds 2 and 3 are maximal (n=99, all
floors except f pressed) with f at an extreme floor and a middle floor,
respectively, and different press orders. Later seeds use random subsets,
buttons clustered to one side of f, and alternating-side press orders.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(f, buttons):
    print(len(buttons), f)
    print(" ".join(map(str, buttons)))


if seed == 1:
    emit(5, [10])
elif seed == 2:
    f = 1
    buttons = [v for v in range(1, 101) if v != f]  # 99 buttons
    rng.shuffle(buttons)
    emit(f, buttons)
elif seed == 3:
    f = 50
    buttons = [v for v in range(1, 101) if v != f]
    rng.shuffle(buttons)
    emit(f, buttons)
elif seed == 4:
    # all buttons strictly above f, pressed in descending order
    f = 10
    buttons = list(range(100, f, -1))
    emit(f, buttons)
elif seed == 5:
    # alternating sides around f to force long back-and-forth runs
    f = 50
    lo = list(range(49, 0, -1))
    hi = list(range(51, 101))
    buttons = []
    for a, b in zip(lo, hi):
        buttons.append(a)
        buttons.append(b)
    emit(f, buttons)
else:
    f = rng.randint(1, 100)
    pool = [v for v in range(1, 101) if v != f]
    n = rng.randint(1, len(pool))
    buttons = rng.sample(pool, n)
    emit(f, buttons)
