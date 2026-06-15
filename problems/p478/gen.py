"""Tests for p478 (Excursion): a, b <= 100 students, n <= 100 notes 0..200.

Seed 1 is minimal, seed 2 is a maximal feasible random walk (a = b = 100,
n = 100), seed 3 is maximal with fully random notes (usually ERROR).
Later seeds mix feasible walks, near-feasible walks with one corrupted
note, and boundary note values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def feasible_notes(rng, a, b, n):
    boys, girls = 0, b
    notes = []
    for _ in range(n):
        boys += rng.randint(0, a - boys)
        girls -= rng.randint(0, girls)
        notes.append(boys + girls)
    return notes


if seed == 1:
    print(1, 1)
    print(1)
    print(1)
elif seed == 2:
    notes = feasible_notes(rng, 100, 100, 100)
    print(100, 100)
    print(100)
    print(*notes)
elif seed == 3:
    print(100, 100)
    print(100)
    print(*[rng.randint(0, 200) for _ in range(100)])
elif seed == 4:
    # Feasible max-size walk with one randomly corrupted note.
    notes = feasible_notes(rng, 100, 100, 100)
    notes[rng.randrange(100)] = rng.randint(0, 200)
    print(100, 100)
    print(100)
    print(*notes)
elif seed == 5:
    # Constant notes equal to b: girls may swap out for boys one-for-one.
    print(100, 100)
    print(100)
    print(*[100] * 100)
elif seed == 6:
    # All notes zero except a late spike (boys can only increase).
    notes = [0] * 100
    notes[70] = rng.randint(1, 200)
    print(100, 100)
    print(100)
    print(*notes)
else:
    a = rng.randint(1, 100)
    b = rng.randint(1, 100)
    n = rng.randint(1, 100)
    if rng.random() < 0.5:
        notes = feasible_notes(rng, a, b, n)
    else:
        notes = [rng.randint(0, 200) for _ in range(n)]
    print(a, b)
    print(n)
    print(*notes)
