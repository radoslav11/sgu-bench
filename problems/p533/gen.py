"""Tests for p533 (Dice Tower): given target surface sum n, find min dice.

Constraint: 1 <= n <= 1e6. The whole answer depends only on n mod 14 and the
magnitude, so we cover boundaries, the special n=21 case, residues that map
to -1, and the maximum.

Seeds: 1 minimal, 2 and 3 maximal, rest hit the structurally interesting n.
"""

import random
import sys

NMAX = 10**6


def emit(n):
    sys.stdout.write(f"{n}\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1)            # minimal n -> -1
elif seed == 2:
    emit(NMAX)         # maximal n
elif seed == 3:
    # Largest n that yields -1 via residue (rem == 13): 1000000 % 14 == 12,
    # so pick the largest n <= 1e6 with n % 14 == 13.
    n = NMAX
    while n % 14 != 13:
        n -= 1
    emit(n)
elif seed == 4:
    emit(21)           # special single-die case -> 1
elif seed == 5:
    emit(7)            # classic -1 (k=0)
elif seed == 6:
    emit(50)           # sample -> 3
elif seed == 7:
    emit(32)           # sample -> 2
elif seed == 8:
    # k=1 boundary: n=28 -> rem=0, but k=2; n in [14,27] is k=1 -> mostly -1.
    emit(15)
elif seed == 9:
    # A residue giving valid answer near the top: largest n with rem==7.
    n = NMAX
    while n % 14 != 7:
        n -= 1
    emit(n)
else:
    emit(rng.randint(1, NMAX))
