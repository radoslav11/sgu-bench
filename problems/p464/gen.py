"""Tests for p464 (Optimal bribing).

One line: N F1 F2 V with 1 <= N <= 10, 0 < F1, F2 < 1 (at most two
digits after the decimal point, so 0.01..0.99), 1 <= V <= 100.

Seed 1 is the minimal race (N=1, tiny stakes), seeds 2-3 the maximal
N=10 races with extreme fascination values, later seeds randomize and
include equal-F ties and strongly asymmetric players.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def f():
    return f"0.{rng.randint(1, 99):02d}"


if seed == 1:
    print("1 0.01 0.01 1")
elif seed == 2:
    print("10 0.99 0.99 100")
elif seed == 3:
    print("10 0.99 0.01 100")
elif seed == 4:
    print("10 0.50 0.50 100")
elif seed == 5:
    x = f()
    print(f"{rng.randint(2, 10)} {x} {x} {rng.randint(1, 100)}")
else:
    print(f"{rng.randint(1, 10)} {f()} {f()} {rng.randint(1, 100)}")
