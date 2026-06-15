"""Tests for p339 (Segments): up to 250000 '+ L R' / '- L R' queries,
-1e9 <= L < R <= 1e9, at most 1000 segments present simultaneously.

Seed 1 is the single-query minimum. Seeds 2-3 are maximal: 250000 queries
that keep the active set pinned at 1000 segments so every '+' pays a full
scan; seed 2 uses wide nested segments (high containment counts), seed 3
hammers a tiny coordinate set with duplicates and bogus removals. Later
seeds are random mid-size mixes with nesting chains, duplicate segments,
removals of absent segments and extreme coordinates.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 10**9
MAXQ = 250000
MAXACT = 1000

out = []


def emit(t, l, r):
    out.append(f"{t} {l} {r}")


if seed == 1:
    emit("+", -MAXC, MAXC)
elif seed == 2:
    # Nested telescope: fill with 1000 nested segments, then keep churning
    # at full occupancy with containing segments so counts stay near 1000.
    active = []
    for i in range(MAXACT):
        l = -MAXC + i * 1000 + rng.randint(0, 999)
        r = MAXC - i * 1000 - rng.randint(0, 999)
        emit("+", l, r)
        active.append((l, r))
    q = MAXACT
    while q + 2 <= MAXQ:
        idx = rng.randrange(len(active))
        l, r = active[idx]
        emit("-", l, r)
        active[idx] = active[-1]
        active.pop()
        nl = rng.randint(-MAXC, -MAXC // 2)
        nr = rng.randint(MAXC // 2, MAXC)
        emit("+", nl, nr)
        active.append((nl, nr))
        q += 2
elif seed == 3:
    # Tiny coordinate universe: heavy duplication, equal segments, removals
    # of segments that are not there, occupancy held at the 1000 cap.
    coords = list(range(-6, 7))
    active = []
    q = 0
    while q < MAXQ:
        if len(active) == MAXACT or (active and rng.random() < 0.45):
            if rng.random() < 0.25:
                l, r = sorted(rng.sample(coords, 2))
                emit("-", l, r)  # may or may not exist
                if (l, r) in active:
                    active.remove((l, r))
            else:
                i = rng.randrange(len(active))
                l, r = active[i]
                emit("-", l, r)
                active[i] = active[-1]
                active.pop()
        else:
            l, r = sorted(rng.sample(coords, 2))
            emit("+", l, r)
            active.append((l, r))
        q += 1
else:
    # Random mid-size mixes; even seeds favour nesting chains, odd seeds
    # favour duplicates, with extreme +-1e9 endpoints sprinkled in.
    q_total = rng.randint(2000, 60000)
    cap = rng.randint(50, MAXACT)
    active = []
    span = rng.choice([20, 1000, MAXC])
    while len(out) < q_total:
        roll = rng.random()
        if active and (len(active) == cap or roll < 0.35):
            if rng.random() < 0.15:
                l = rng.randint(-span, span - 1)
                r = rng.randint(l + 1, span)
                emit("-", l, r)
                if (l, r) in active:
                    active.remove((l, r))
            else:
                i = rng.randrange(len(active))
                l, r = active[i]
                emit("-", l, r)
                active[i] = active[-1]
                active.pop()
        else:
            mode = rng.random()
            if mode < 0.2 and active:
                # nest strictly inside an existing segment when possible
                l0, r0 = active[rng.randrange(len(active))]
                if r0 - l0 >= 3:
                    l = rng.randint(l0, r0 - 2)
                    r = rng.randint(l + 1, r0)
                else:
                    l, r = l0, r0
            elif mode < 0.35 and active:
                l, r = active[rng.randrange(len(active))]  # duplicate
            elif mode < 0.45:
                l, r = -MAXC, MAXC
            else:
                l = rng.randint(-span, span - 1)
                r = rng.randint(l + 1, span)
            emit("+", l, r)
            active.append((l, r))

print("\n".join(out))
