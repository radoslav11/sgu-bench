"""Tests for p133 (Border): count intervals strictly nested in another.

N <= 16000 intervals [A, B], 0 <= A < B <= 2e9, all A distinct, all B
distinct. Seed 1 is a single interval, seed 2 a maximal fully nested
family (every non-outer interval redundant), seed 3 a maximal disjoint
staircase (none redundant). Later seeds mix random and adversarial
overlap patterns.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 2_000_000_000
NMAX = 16000


def emit(intervals):
    rng.shuffle(intervals)
    print(len(intervals))
    for a, b in intervals:
        print(a, b)


if seed == 1:
    print(1)
    print(0, MAXC)
    sys.exit()

if seed == 2:
    # Fully nested: [i, MAXC - i] for i in 0..N-1.
    emit([(i, MAXC - i) for i in range(NMAX)])
    sys.exit()

if seed == 3:
    # Staircase of overlapping but non-nested intervals.
    intervals = []
    for i in range(NMAX):
        a = i * 100000
        b = a + 150000
        intervals.append((a, b))
    emit(intervals)
    sys.exit()


def random_intervals(n):
    avals = rng.sample(range(0, MAXC), n)
    intervals = []
    used_b = set()
    for a in avals:
        b = rng.randint(a + 1, MAXC)
        while b in used_b:
            b = rng.randint(a + 1, MAXC)
        used_b.add(b)
        intervals.append((a, b))
    return intervals


if seed == 4:
    # Random sorted-A intervals with random nesting, max size.
    emit(random_intervals(NMAX))
    sys.exit()

n = rng.randint(2, 5000)
style = rng.randrange(3)
if style == 0:
    intervals = random_intervals(n)
elif style == 1:
    # Mostly nested chains glued side by side.
    intervals = []
    base = 0
    while len(intervals) < n:
        depth = min(rng.randint(1, 50), n - len(intervals))
        width = 4 * depth + rng.randint(2, 1000)
        for d in range(depth):
            intervals.append((base + d, base + width - d))
        base += width + rng.randint(1, 10)
else:
    # Short intervals around random centres (many partial overlaps).
    intervals = []
    used_a, used_b = set(), set()
    while len(intervals) < n:
        c = rng.randint(1000, MAXC - 1000)
        a = c - rng.randint(1, 900)
        b = c + rng.randint(1, 900)
        if a in used_a or b in used_b:
            continue
        used_a.add(a)
        used_b.add(b)
        intervals.append((a, b))
emit(intervals)
