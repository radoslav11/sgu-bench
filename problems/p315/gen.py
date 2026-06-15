"""Tests for p315 (The Highway Belt): 1 <= N <= 50 segments, integer
endpoints with |x|, |y| <= 100, the capital at the origin.

Seed 1 is a single segment, seed 2 is the maximal 50-segment case built
from a dense bundle of lines crossing the origin neighbourhood (many
intersections), seed 3 is another maximal case with segments arranged on a
grid of crossing lines, the rest are randomized small/medium cases plus
adversarial patterns (segments through the origin, collinear overlaps,
zero-length segments).
"""

import random
import sys

C = 100
NMAX = 50

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(segs):
    out = [str(len(segs))]
    for (x1, y1, x2, y2) in segs:
        out.append(f"{x1} {y1} {x2} {y2}")
    sys.stdout.write("\n".join(out) + "\n")


def rand_pt():
    return rng.randint(-C, C), rng.randint(-C, C)


def rand_seg():
    return (*rand_pt(), *rand_pt())


if seed == 1:
    emit([(1, 1, -1, -1)])
elif seed == 2:
    # 50 long chords across the full square: dense web of intersections
    # surrounding the origin, forcing the large star-belt DP.
    segs = []
    while len(segs) < NMAX:
        x1, y1 = rng.randint(-C, C), rng.randint(-C, C)
        x2, y2 = rng.randint(-C, C), rng.randint(-C, C)
        if (x1, y1) != (x2, y2):
            segs.append((x1, y1, x2, y2))
    emit(segs)
elif seed == 3:
    # Crossing grid lines: a bundle of near-axis-parallel chords so that
    # most pairs intersect inside the square.
    segs = []
    for k in range(-C, C + 1, 8):
        segs.append((-C, k, C, k))   # horizontal
        if len(segs) >= NMAX:
            break
    i = 0
    while len(segs) < NMAX:
        k = -C + i * 8
        segs.append((k, -C, k, C))   # vertical
        i += 1
    emit(segs[:NMAX])
elif seed == 4:
    # Star of chords radiating around the origin (each not through it).
    segs = []
    for k in range(NMAX):
        a = k * 7 % 50 - 25
        segs.append((a, -C, a + 1, C))
    emit(segs)
elif seed == 5:
    # Many segments passing through the origin (must be discarded) mixed in.
    segs = []
    for k in range(1, 11):
        segs.append((-k, -k, k, k))      # through origin
    while len(segs) < NMAX:
        segs.append(rand_seg())
    emit(segs[:NMAX])
elif seed == 6:
    # Collinear overlapping segments plus zero-length segments.
    segs = [(0, 5, 100, 5), (50, 5, -100, 5), (5, 0, 5, 0)]
    while len(segs) < NMAX:
        segs.append(rand_seg())
    emit(segs[:NMAX])
elif seed == 7:
    n = rng.randint(2, 8)
    segs = [tuple(rng.randint(-10, 10) for _ in range(4)) for _ in range(n)]
    emit(segs)
elif seed == 8:
    n = rng.randint(20, NMAX)
    segs = [rand_seg() for _ in range(n)]
    emit(segs)
else:
    n = rng.randint(5, NMAX)
    # small-coordinate dense case to maximize coincident points / ties
    segs = [tuple(rng.randint(-4, 4) for _ in range(4)) for _ in range(n)]
    emit(segs)
