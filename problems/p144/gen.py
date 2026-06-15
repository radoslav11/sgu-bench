"""Tests for p144 (Meeting): 0 <= X < Y <= 24, 0 < Z <= 60*(Y-X).

The input is three numbers, so every test is maximal-size; the seeds
instead cover the parameter space. Seed 1 is the tightest window with a
tiny Z, seed 2 the widest window with Z at its cap (probability 1),
seed 3 the widest window with a tiny Z. Later seeds randomize hours and
Z, mixing integer-valued and fractional Z.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    x, y, z = 0, 1, 0.01
elif seed == 2:
    x, y, z = 0, 24, 1440.0
elif seed == 3:
    x, y, z = 0, 24, 0.0001
elif seed == 4:
    x, y, z = 11, 12, 20.0  # statement sample
elif seed == 5:
    x, y, z = 23, 24, 60.0  # Z exactly equal to the window
else:
    x = rng.randint(0, 23)
    y = rng.randint(x + 1, 24)
    cap = 60 * (y - x)
    if seed % 2 == 0:
        z = float(rng.randint(1, cap))
    else:
        z = round(rng.uniform(0.001, cap), 4)

print(x, y, f"{z:.4f}")
