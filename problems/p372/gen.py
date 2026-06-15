"""Tests for p372 (Tea Party): first line "K N", then N lines "ci si".

1 <= K, N <= 1000; si is 0 (green) or 1 (black). The ci bound was lost
from the statement; tests use 1 <= ci <= 10^9, which forces 64-bit cost
accumulation (sum can reach ~5*10^14). Seed 1 is minimal, seed 2 is the
maximal uniform-random case, seed 3 a maximal kind-skewed case (667
black / 333 green, exactly on the feasibility edge), then Impossible
shapes (all one kind, K > N, too skewed) and tie-heavy cases.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(k, teas):
    out = [f"{k} {len(teas)}"]
    out += [f"{c} {s}" for c, s in teas]
    sys.stdout.write("\n".join(out) + "\n")


if seed == 1:
    emit(1, [(5, 0)])
elif seed == 2:
    emit(
        1000,
        [(rng.randint(1, 10**9), rng.randint(0, 1)) for _ in range(1000)],
    )
elif seed == 3:
    teas = [(rng.randint(1, 10**9), 1) for _ in range(667)]
    teas += [(rng.randint(1, 10**9), 0) for _ in range(333)]
    rng.shuffle(teas)
    emit(1000, teas)
elif seed == 4:
    emit(1000, [(rng.randint(1, 10**9), 1) for _ in range(1000)])
elif seed == 5:
    emit(1000, [(rng.randint(1, 10**9), rng.randint(0, 1)) for _ in range(500)])
elif seed == 6:
    # Huge tie blocks: only three distinct costs across 1000 teas.
    teas = [(rng.choice([7, 10**9, 123456]), rng.randint(0, 1)) for _ in range(1000)]
    emit(1000, teas)
elif seed == 7:
    emit(3, [(1, 0), (2, 0), (4, 1), (3, 1)])  # the statement's sample
elif seed == 8:
    # Too skewed to interleave: 900 black, 100 green, K = 1000.
    teas = [(rng.randint(1, 10**9), 1) for _ in range(900)]
    teas += [(rng.randint(1, 10**9), 0) for _ in range(100)]
    rng.shuffle(teas)
    emit(1000, teas)
else:
    n = rng.randint(1, 1000)
    k = rng.randint(1, 1000)
    p_black = rng.random()
    hi = rng.choice([50, 10**4, 10**9])
    teas = [
        (rng.randint(1, hi), 1 if rng.random() < p_black else 0)
        for _ in range(n)
    ]
    emit(k, teas)
