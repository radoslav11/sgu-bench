"""Tests for p202 (Towers of Hanoi Revisited): 1 <= N <= 64, 4 <= M <= 65.

Seed 1 is minimal (N=1, M=4). Seed 2 is N=64, M=4 — the longest possible
move sequence. Seed 3 is N=64, M=65 (maximal pegs, one aside per peg).
Seeds 4+ cover N=64 with few pegs and random combinations.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

cases = {
    1: (1, 4),
    2: (64, 4),
    3: (64, 65),
    4: (64, 5),
    5: (63, 4),
}

if seed in cases:
    n, m = cases[seed]
else:
    n = rng.randint(1, 64)
    m = rng.randint(4, 65)

print(n, m)
