"""Tests for p150 (Mr. Beetle II): integer x1 y1 x2 y2 n on one line,
|coords| <= 10^6, 1 <= n <= 10^5.

Seed 1 is a degenerate point. Seeds 2-3 are maximal-length segments with
n = 10^5: the main diagonal (lattice-corner crossing at every step) and a
coprime-slope line (no lattice crossings). Later seeds cover axis-aligned
segments (boundary riding -> no cells), corner-heavy slopes, "no solution"
via short segments, and exact cell-count boundaries.
"""

import random
import sys

C = 10**6
NMAX = 10**5

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0, 0, 0, 0, 1)
elif seed == 2:
    print(-C, -C, C, C, NMAX)
elif seed == 3:
    print(-C, -C, C, C - 1, NMAX)
elif seed == 4:
    # slope 1/2: lattice-corner crossing every 2 units of x
    print(-C, -C // 2, C, C // 2, NMAX)
elif seed == 5:
    # horizontal segment rides a grid line: no cell is ever entered
    y = rng.randint(-C, C)
    print(-C, y, C, y, 1)
elif seed == 6:
    # vertical segment rides a grid line
    x = rng.randint(-C, C)
    print(x, -C, x, C, NMAX)
elif seed == 7:
    # short segment, huge n -> no solution
    print(0, 0, 3, 7, NMAX)
elif seed == 8:
    # n exactly the number of visited cells of a small segment
    # segment (0,0)-(4,6): cells = 4+6-gcd(4,6) = 8
    print(0, 0, 4, 6, 8)
elif seed == 9:
    # one past the last cell -> no solution
    print(0, 0, 4, 6, 9)
elif seed == 10:
    # degenerate point with n=1 (still no solution: point is a corner)
    x, y = rng.randint(-C, C), rng.randint(-C, C)
    print(x, y, x, y, 1)
else:
    # random segments, mixed n
    x1, y1 = rng.randint(-C, C), rng.randint(-C, C)
    x2, y2 = rng.randint(-C, C), rng.randint(-C, C)
    n = rng.choice([1, rng.randint(1, 100), rng.randint(1, NMAX)])
    print(x1, y1, x2, y2, n)
