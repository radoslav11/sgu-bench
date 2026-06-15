"""Tests for p328 (A Coloring Game): chain of N vertices, digits 0/1/2,
adjacent vertices never share a nonzero color. The original bound on N is
lost from the statement; tests use N up to 100000 which the O(N) scan
handles instantly.

Seed 1 is a single uncolored vertex, seed 2 a maximal random mix of
colored walls and zero segments, seed 3 the maximal all-zero chain.
Later seeds cover a fully colored chain, single-wall boundary segments,
and random stitched match/diff segments.
"""

import random
import sys

MAXN = 100000

seed = int(sys.argv[1])
rng = random.Random(seed)


def random_valid(n, zero_weight):
    """Random chain: each vertex 0 with given weight, else a legal color."""
    out = []
    prev = "0"
    for _ in range(n):
        choices = ["0"] * zero_weight + [
            c for c in "12" if c != prev or prev == "0"
        ]
        c = rng.choice(choices)
        out.append(c)
        prev = c
    return "".join(out)


def emit(s):
    print(len(s))
    print(s)


if seed == 1:
    emit("0")
elif seed == 2:
    emit(random_valid(MAXN, 3))
elif seed == 3:
    emit("0" * MAXN)
elif seed == 4:
    emit("12" * (MAXN // 2))
elif seed == 5:
    # One colored wall: two boundary segments of random lengths.
    left = rng.randint(0, MAXN // 2)
    emit("0" * left + rng.choice("12") + "0" * (MAXN - 1 - left))
elif seed == 6:
    # Stitched segments with explicit match/diff walls of varied lengths.
    chain = "1"
    while len(chain) < MAXN - 60:
        gap = rng.randint(0, 50)
        if gap == 0:
            wall = "2" if chain[-1] == "1" else "1"
        else:
            wall = rng.choice("12")
        chain += "0" * gap + wall
    emit(chain)
elif seed == 7:
    emit(random_valid(rng.randint(1, 50), 2))
else:
    emit(random_valid(rng.randint(100, MAXN), rng.choice([1, 2, 8])))
