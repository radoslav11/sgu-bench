"""Tests for p395 ("Binary Cat" Club): N <= 200 log records.

Records are "+ name", "- name", "= c" with 0 <= c <= 100 and lowercase
names of length 1..16. Any such sequence is a valid damaged log, so the
generator just controls the mix. Seed 1 is a single record, seed 2 a
200-record random mix over a small name pool (lots of repeated enters
and leaves), seed 3 has 200 alternating "= 100" / "= 0" records, which
forces ~100 insertions per step. Later seeds cover one-name logs,
already-plausible logs, and random blends.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

ALPHA = "abcdefghijklmnopqrstuvwxyz"


def name(rng, lo=1, hi=16):
    return "".join(
        rng.choice(ALPHA) for _ in range(rng.randint(lo, hi))
    )


def emit(records):
    print(len(records))
    for r in records:
        print(r)


if seed == 1:
    emit(["- somecat"])
elif seed == 2:
    pool = [name(rng) for _ in range(8)] + ["a", "z" * 16]
    recs = []
    for _ in range(200):
        r = rng.random()
        if r < 0.35:
            recs.append("+ " + rng.choice(pool))
        elif r < 0.7:
            recs.append("- " + rng.choice(pool))
        else:
            recs.append("= " + str(rng.choice([0, 1, 2, 50, 99, 100])))
    emit(recs)
elif seed == 3:
    emit(["= " + ("100" if i % 2 == 0 else "0") for i in range(200)])
elif seed == 4:
    recs = []
    for i in range(200):
        recs.append(("+ " if i % 2 == 0 else "- ") + "silentbob")
    emit(recs)
elif seed == 5:
    # Already-plausible log: stack discipline plus correct counts.
    inside = []
    recs = []
    while len(recs) < 150:
        r = rng.random()
        if r < 0.45 or not inside:
            nm = name(rng, 3, 10) + str(len(recs))
            nm = "".join(ch for ch in nm if ch.isalpha()) or "cat"
            inside.append(nm)
            recs.append("+ " + nm)
        elif r < 0.85:
            recs.append("- " + inside.pop())
        else:
            recs.append("= " + str(min(len(inside), 100)))
    emit(recs)
else:
    n = rng.randint(2, 200)
    pool = [name(rng, 1, 16) for _ in range(rng.randint(1, 30))]
    recs = []
    for _ in range(n):
        r = rng.random()
        if r < 0.4:
            recs.append("+ " + rng.choice(pool))
        elif r < 0.8:
            recs.append("- " + rng.choice(pool))
        else:
            recs.append("= " + str(rng.randint(0, 100)))
    emit(recs)
