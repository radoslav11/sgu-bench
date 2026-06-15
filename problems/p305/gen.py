"""Tests for p305 (Exhibition): assign distinct numbers from [1, M] to N
arithmetic progressions Ai*K+Bi (K >= 0), maximizing how many get a
number that is a member; solved via bipartite matching.

1 <= N <= 300, N <= M <= 1e9, |Ai|, |Bi| <= 1e9. Seed 1 is minimal.
Seeds 2-3 are maximal with heavy contention over few small values
(matching actually matters). Other seeds: full-range random pairs,
mass-duplicated progressions, negative steps, constant progressions
with M = N, mid-size random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

BIG = 10**9


def emit(n, m, prog):
    out = [f"{n} {m}"]
    out += [f"{a} {b}" for a, b in prog]
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, 1, [(0, 1)])
elif seed == 2:
    n = 300
    prog = [(rng.randint(1, 3), rng.randint(-5, 5)) for _ in range(n)]
    emit(n, BIG, prog)
elif seed == 3:
    n = 300
    prog = [(rng.randint(-20, 20), rng.randint(-50, 400)) for _ in range(n)]
    emit(n, n, prog)
elif seed == 4:
    n = 300
    prog = [
        (rng.randint(-BIG, BIG), rng.randint(-BIG, BIG)) for _ in range(n)
    ]
    prog[0] = (BIG, 1)
    prog[1] = (BIG, BIG)
    prog[2] = (-BIG, BIG)
    prog[3] = (0, BIG)
    prog[4] = (0, 1)
    emit(n, BIG, prog)
elif seed == 5:
    n = 300
    base = [
        (rng.randint(1, 7), rng.randint(-3, 3)) for _ in range(10)
    ]
    prog = [rng.choice(base) for _ in range(n)]
    emit(n, BIG, prog)
elif seed == 6:
    n = 300
    prog = [
        (rng.randint(-BIG, -1), rng.randint(1, BIG)) for _ in range(n)
    ]
    emit(n, 5 * 10**8, prog)
elif seed == 7:
    n = 300
    prog = [(0, rng.randint(-5, 320)) for _ in range(n)]
    emit(n, n, prog)
else:
    n = rng.randint(2, 50)
    m = rng.randint(n, 1000)
    prog = [
        (rng.randint(-30, 30), rng.randint(-100, 100)) for _ in range(n)
    ]
    emit(n, m, prog)
