"""Tests for p273 (Game Po): 1 <= B,R,Y,W <= 16, row of 1..200 stones.

Seed 1 is minimal (one rule per color, one stone). Seed 2 is maximal
with every possible replacement rule and a 200-stone row. Seed 3 is
maximal with a sparse rule set so reachable-color masks stay small (no
early break in interval DP) and "Nobody"/partial answers appear. Other
seeds are random; odd ones bias towards single-color rows and rules
that keep the row irreducible.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

COLORS = "bryw"
ALL_PAIRS = [a + b for a in COLORS for b in COLORS]


def emit(rules_by_color, row):
    print(*(len(rules_by_color[c]) for c in range(4)))
    for c in range(4):
        for rule in rules_by_color[c]:
            print(rule)
    print(row)


if seed == 1:
    emit([["br"], ["yw"], ["wb"], ["ry"]], "b")
elif seed == 2:
    emit([ALL_PAIRS[:] for _ in range(4)],
         "".join(rng.choice(COLORS) for _ in range(200)))
elif seed == 3:
    rules = [[], [], [], []]
    for c in range(4):
        pool = rng.sample(ALL_PAIRS, 3)
        while len(rules[c]) < 16:
            rules[c].append(rng.choice(pool))
    emit(rules, "".join(rng.choice(COLORS) for _ in range(200)))
else:
    rules = [[], [], [], []]
    for c in range(4):
        k = rng.randint(1, 16)
        rules[c] = [rng.choice(ALL_PAIRS) for _ in range(k)]
    if seed % 2 == 1:
        ch = rng.choice(COLORS)
        row = "".join(
            ch if rng.random() < 0.9 else rng.choice(COLORS)
            for _ in range(rng.randint(1, 200))
        )
    else:
        row = "".join(
            rng.choice(COLORS) for _ in range(rng.randint(1, 200))
        )
    emit(rules, row)
