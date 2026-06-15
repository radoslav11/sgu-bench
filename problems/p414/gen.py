"""Tests for p414 (Orthogonal Circles): n circles, find the circle orthogonal
to all of them (unique -> its (x,y,r), none -> -1, many -> -2).

Two circles are orthogonal iff the squared center distance equals the sum of
squared radii. For a chosen target circle C=(X,Y,R) the circles
(X+s, Y+R, |s|) and (X+R, Y+s, |s|) are orthogonal to C for any integer s!=0,
so we can emit arbitrarily many integer circles with a known unique answer.
Mixing the two families keeps the coefficient matrix full rank.

Coordinates and radii are kept small (a few thousand). The reference solves
the linear system in long double, and very large coordinates make the
elimination lose precision; staying small keeps the reference's verdict exact
while still exercising every code path. Circles may coincide, so maximal-count
tests repeat circles to reach n = 100000.

Seed 1 is the minimal single-circle case (-2). Seeds 2 and 3 are maximal
100000-circle unique-answer tests with different centers. Later seeds cover
-1, -2, coincident duplicates, and random valid mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LIM = 10**6
COORD = 4000  # keep |x|,|y|,r within this so the long-double reference is exact


def orthogonal_circles(X, Y, R, count, smax, allow_dup):
    # Emit `count` integer circles orthogonal to C=(X,Y,R), mixing the two
    # families. Both families are always present so the system is full rank.
    out = []
    used = set()
    fams = [True, False]
    # Force one of each family up front.
    for fam in fams:
        s = rng.randint(1, smax)
        if fam:
            c = (X + s, Y + R, abs(s))
        else:
            c = (X + R, Y + s, abs(s))
        out.append(c)
        used.add(c)
    tries = 0
    while len(out) < count and tries < count * 30 + 1000:
        tries += 1
        s = rng.randint(1, smax)
        if rng.random() < 0.5:
            s = -s
        if rng.random() < 0.5:
            c = (X + s, Y + R, abs(s))
        else:
            c = (X + R, Y + s, abs(s))
        if abs(c[0]) > LIM or abs(c[1]) > LIM or not (1 <= c[2] <= LIM):
            continue
        if not allow_dup and c in used:
            continue
        used.add(c)
        out.append(c)
    return out


def fill_to(circ, count):
    # Pad with repeats (circles may coincide) until reaching count.
    if not circ:
        return circ
    while len(circ) < count:
        circ.append(circ[rng.randrange(len(circ))])
    return circ


def emit(circles):
    lines = [str(len(circles))]
    for c in circles:
        lines.append(f"{c[0]} {c[1]} {c[2]}")
    sys.stdout.write("\n".join(lines) + "\n")


def unique_test(count, X, Y, R, smax):
    circ = orthogonal_circles(X, Y, R, count, smax, allow_dup=True)
    circ = fill_to(circ, count)
    rng.shuffle(circ)
    return circ


if seed == 1:
    # One circle: infinitely many orthogonal circles -> -2.
    emit([(0, 0, 5)])
    sys.exit()

if seed == 2:
    # Maximal unique-answer test.
    emit(unique_test(100000, 0, 0, 2000, 2000))
    sys.exit()

if seed == 3:
    # Second maximal unique-answer test, off-center.
    emit(unique_test(100000, -1500, 1200, 1500, 2400))
    sys.exit()

if seed == 4:
    # No orthogonal circle (-1): two coincident-center circles of different
    # radii give contradictory equations.
    emit([(0, 0, 3), (0, 0, 7), (10, 0, 4)])
    sys.exit()

if seed == 5:
    # All centers collinear on the x axis: rank 2, here inconsistent -> -1.
    circ = []
    for _ in range(50):
        x = rng.randint(-COORD, COORD)
        r = rng.randint(1, 2000)
        circ.append((x, 0, r))
    emit(circ)
    sys.exit()

if seed == 6:
    # Coincident circles plus a valid unique answer.
    base = unique_test(10, 100, 200, 1500, 2000)
    circ = base + [base[0]] * 5 + [base[1]] * 5
    rng.shuffle(circ)
    emit(circ)
    sys.exit()

if seed == 7:
    # Two circles only: rank 2, infinitely many -> -2.
    emit([(0, 0, 100), (300, 0, 200)])
    sys.exit()

if seed == 8:
    # Many identical circles -> -2 (rank 1, consistent).
    emit(fill_to([(7, 9, 11)], rng.randint(1000, 100000)))
    sys.exit()

if seed == 9:
    # Another -1 via two coincident-center circles inside a larger system.
    circ = [(5, 5, 2), (5, 5, 9)] + unique_test(20, 5, 5, 1000, 1500)
    rng.shuffle(circ)
    emit(circ)
    sys.exit()

# Random valid unique-answer mid-size mixes (kept in the safe coordinate range).
n = rng.randint(3, 20000)
X = rng.randint(-1000, 1000)
Y = rng.randint(-1000, 1000)
R = rng.randint(2, 2000)
smax = rng.randint(2, 2500)
emit(unique_test(n, X, Y, R, smax))
