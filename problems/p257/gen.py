"""Tests for p257 (Debt): debts P,O,S in [1,1e5], N <= 1e5 crystals,
each line three letters B/S (estimates by Popov, Parkin, Studnev).

Seed 1 is minimal (N=1). Seed 2 is maximal and infeasible (debt sum
exceeds any achievable value), seed 3 is maximal and feasible by
construction (debts set to the value of a random partition). Seeds 4-5
sit exactly on the feasibility boundary with all-small crystals.
Later seeds are random mid-size mixes with skewed type distributions.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXV = 10**5


def emit(p, o, s, crystals):
    out = [f"{p} {o} {s}", str(len(crystals))]
    out += crystals
    sys.stdout.write("\n".join(out) + "\n")


def rand_crystal():
    return "".join(rng.choice("BS") for _ in range(3))


if seed == 1:
    emit(1, 1, 1, ["BSB"])
elif seed == 2:
    emit(MAXV, MAXV, MAXV, [rand_crystal() for _ in range(MAXV)])
elif seed == 3:
    crystals = [rand_crystal() for _ in range(MAXV)]
    received = [0, 0, 0]
    for c in crystals:
        who = rng.randrange(3)
        received[who] += 2 if c[who] == "B" else 1
    p, o, s = (max(1, min(MAXV, v)) for v in received)
    emit(p, o, s, crystals)
elif seed == 4:
    n = MAXV
    a = n // 3
    emit(a, a, n - 2 * a, ["SSS"] * n)
elif seed == 5:
    n = MAXV
    a = n // 3
    emit(a, a, n - 2 * a + 1, ["SSS"] * n)
else:
    n = rng.randint(1, MAXV)
    skew = rng.random()
    types = ["BBB", "SSS", "BSS", "SBS", "SSB", "BBS", "BSB", "SBB"]
    weights = [rng.random() ** (3 * skew) for _ in types]
    crystals = rng.choices(types, weights=weights, k=n)
    if rng.random() < 0.5:
        received = [0, 0, 0]
        for c in crystals:
            who = rng.randrange(3)
            received[who] += 2 if c[who] == "B" else 1
        p, o, s = (max(1, min(MAXV, v)) for v in received)
    else:
        p, o, s = (rng.randint(1, MAXV) for _ in range(3))
    emit(p, o, s, crystals)
