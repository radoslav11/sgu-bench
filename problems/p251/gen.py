"""Tests for p251 (Polymania): N in [3,8], special numbers R[i] in [1,100]
with R[N-1] = R[N]; find points so every triangle area equals the sum of
its three special numbers.

Seed 1 is the minimal all-ones triangle, seeds 2-3 are N=8 (largest,
answer NO), then feasible and infeasible N=4 patterns (feasible needs
R1 = R2 or R1 + R2 = 2*R3), N=3 maxima, and random cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(vals):
    print(len(vals))
    print(*vals)


if seed == 1:
    emit([1, 1, 1])
elif seed == 2:
    v = [rng.randint(1, 100) for _ in range(7)]
    emit(v + [v[-1]])
elif seed == 3:
    emit([100] * 8)
elif seed == 4:
    emit([7, 7, 13, 13])
elif seed == 5:
    emit([1, 3, 2, 2])
elif seed == 6:
    emit([1, 2, 5, 5])
elif seed == 7:
    emit([100, 100, 100])
elif seed == 8:
    emit([1, 100, 50, 50])
else:
    n = rng.randint(3, 8)
    v = [rng.randint(1, 100) for _ in range(n - 1)]
    emit(v + [v[-1]])
