"""Tests for p548 (Dragons and Princesses).

Input: line 1 n (2<=n<=2e5). Then n-1 lines for cells 2..n: "d g"
(dragon, 1<=g<=1e4) or "p b" (princess, 1<=b<=2e5). The last cell is a
princess.

Seed 1 is the minimal n=2 case. Seeds 2-3 are maximal n=2e5: seed 2 is a
long run of dragons ending in one princess (the heap holds ~n elements);
seed 3 alternates dragons and demanding princesses so the greedy is forced
to drop many cheapest kills. Later seeds mix random cell types/values,
guaranteed-infeasible cases, and adversarial beauty patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

NMAX = 200000
GMAX = 10000
BMAX = 200000


def emit(n, cells):
    # cells: list of (type, value) for cells 2..n, length n-1
    out = [str(n)]
    for t, v in cells:
        out.append(f"{t} {v}")
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    # minimal: n=2, only the last princess. Beauty 1 but 0 dragons -> -1
    emit(2, [("p", 1)])
    sys.exit()

if seed == 2:
    # max: dragons in cells 2..n-1, last princess with a modest beauty so
    # the answer is feasible (sum of all dragons).
    n = NMAX
    cells = [("d", rng.randint(1, GMAX)) for _ in range(n - 2)]
    cells.append(("p", rng.randint(1, n - 2)))
    emit(n, cells)
    sys.exit()

if seed == 3:
    # max: alternating dragons and princesses with rising beauty demands,
    # forcing the greedy to evict cheap kills repeatedly.
    n = NMAX
    cells = []
    killed_estimate = 0
    for i in range(2, n):
        if i % 2 == 0:
            cells.append(("d", rng.randint(1, GMAX)))
            killed_estimate += 1
        else:
            # beauty that occasionally trims the heap
            cells.append(("p", rng.randint(1, max(1, killed_estimate))))
    # last princess has beauty 1, so any single surviving kill suffices:
    # the case stays feasible while exercising heavy heap eviction.
    cells.append(("p", 1))
    emit(n, cells)
    sys.exit()

if seed == 4:
    # guaranteed infeasible: last princess beauty exceeds number of dragons
    n = 1000
    cells = []
    dragons = 0
    for i in range(2, n):
        if rng.random() < 0.5:
            cells.append(("d", rng.randint(1, GMAX)))
            dragons += 1
        else:
            cells.append(("p", rng.randint(1, 10)))
    cells.append(("p", dragons + 100))
    emit(n, cells)
    sys.exit()

if seed == 5:
    # many princesses with beauty 1 right after dragons: forces dropping
    n = 50000
    cells = []
    for i in range(2, n):
        if i % 3 == 0:
            cells.append(("p", 1))
        else:
            cells.append(("d", rng.randint(1, GMAX)))
    cells.append(("p", 1))
    emit(n, cells)
    sys.exit()

# random mid-size
n = rng.randint(2, 5000)
cells = []
for i in range(2, n):
    if rng.random() < 0.6:
        cells.append(("d", rng.randint(1, GMAX)))
    else:
        cells.append(("p", rng.randint(1, n)))
cells.append(("p", rng.randint(1, n)))
emit(n, cells)
