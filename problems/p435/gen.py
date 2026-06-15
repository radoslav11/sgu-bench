"""Tests for p435 (UFO Circles): N <= 100, |x|, |y|, r <= 1000, r > 0.

Seed 1 is a single circle. Seed 2 is 100 circles on a dense grid where
every circle overlaps its neighbours (maximum number of arcs). Seed 3 is
100 big circles crammed into a small area (deep overlap parity). Other
seeds cover concentric rings, tangent pairs, disjoint circles and random
configurations. All emitted (x, y, r) triples are distinct.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(circles):
    # All circles must be pairwise different.
    assert len(set(circles)) == len(circles)
    print(len(circles))
    for x, y, r in circles:
        print(x, y, r)


if seed == 1:
    emit([(0, 0, 1000)])
elif seed == 2:
    # 10 x 10 grid, spacing 150, radii ~100..140: each circle crosses
    # its grid neighbours, producing the densest arc structure.
    circles = []
    for i in range(10):
        for j in range(10):
            circles.append((-675 + 150 * i, -675 + 150 * j,
                            100 + (i * 10 + j) % 41))
    emit(circles)
elif seed == 3:
    # 100 large circles with centers packed into a small disc: every
    # pair intersects, coverage parity gets deep.
    circles = set()
    while len(circles) < 100:
        circles.add((rng.randint(-100, 100), rng.randint(-100, 100),
                     rng.randint(700, 1000)))
    emit(sorted(circles))
elif seed == 4:
    # Concentric rings: no intersections, alternating parity annuli.
    emit([(0, 0, r) for r in range(50, 1001, 50)])
elif seed == 5:
    # Tangent chains: externally tangent pairs and one internal tangency.
    circles = [(-900 + 200 * i, 0, 100) for i in range(10)]
    circles += [(0, 500, 100), (0, 450, 50)]
    circles += [(-900 + 200 * i, -400, 100) for i in range(10)]
    emit(circles)
elif seed == 6:
    # Disjoint far-apart circles: answer is just the sum of areas.
    circles = []
    for i in range(5):
        for j in range(5):
            circles.append((-800 + 400 * i, -800 + 400 * j, rng.randint(1, 90)))
    emit(circles)
else:
    n = rng.randint(2, 100)
    circles = set()
    while len(circles) < n:
        circles.add((rng.randint(-1000, 1000), rng.randint(-1000, 1000),
                     rng.randint(1, rng.choice([20, 200, 1000]))))
    emit(sorted(circles))
