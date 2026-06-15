"""Tests for p110 (Dungeon): n <= 50 spheres, integer centers/radii up to
1e4 in absolute value, ray start strictly outside all spheres.

Spheres are pairwise disjoint (rejection sampling) and the ray is aimed
well inside a sphere (not tangentially), so the hit sequence is
numerically stable. Seed 1 is one sphere hit head-on, seed 2 a maximal
50-sphere scene, seed 3 a maximal scene with two facing spheres planted
on the ray (infinite bouncing, "etc."), seed 4 a ray that misses
everything, seed 5 an exact tangent hit (disc == 0), the rest random.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def disjoint_spheres(count, lo=-9000, hi=9000, rmin=50, rmax=800,
                     planted=()):
    spheres = list(planted)
    while len(spheres) < count:
        x, y, z = (rng.randint(lo, hi) for _ in range(3))
        r = rng.randint(rmin, rmax)
        ok = True
        for (sx, sy, sz, sr) in spheres:
            d2 = (x - sx) ** 2 + (y - sy) ** 2 + (z - sz) ** 2
            if d2 <= (r + sr + 5) ** 2:
                ok = False
                break
        if ok:
            spheres.append((x, y, z, r))
    return spheres


def emit(spheres, shot):
    print(len(spheres))
    for s in spheres:
        print(*s)
    print(*shot)


if seed == 1:
    emit([(0, 0, 2, 1)], (0, 0, 0, 0, 0, 1))
elif seed == 2:
    spheres = disjoint_spheres(50)
    tx, ty, tz, tr = rng.choice(spheres)
    emit(spheres, (10000, 10000, 10000, tx, ty, tz))
elif seed == 3:
    planted = [(0, 0, 3000, 1000), (0, 0, -3000, 1000)]
    spheres = disjoint_spheres(50, planted=planted)
    rng.shuffle(spheres)
    emit(spheres, (0, 0, 0, 0, 0, 100))
elif seed == 4:
    spheres = disjoint_spheres(50, lo=-9000, hi=-100, rmax=400)
    emit(spheres, (10000, 10000, 10000, 10000, 10000, 9999))
elif seed == 5:
    emit([(0, 0, 2, 1)], (-10, 0, 1, 10, 0, 1))
else:
    n = rng.randint(2, 50)
    spheres = disjoint_spheres(n)
    tx, ty, tz, tr = rng.choice(spheres)
    ox, oy, oz = (rng.randint(-10000, 10000) for _ in range(3))
    while any((ox - sx) ** 2 + (oy - sy) ** 2 + (oz - sz) ** 2
              <= (sr + 2) ** 2 for sx, sy, sz, sr in spheres):
        ox, oy, oz = (rng.randint(-10000, 10000) for _ in range(3))
    # Aim at a point well inside the chosen sphere (at most r/2 off
    # center) so the first hit is never a grazing one.
    ax = tx + rng.uniform(-tr / 2, tr / 2)
    ay = ty + rng.uniform(-tr / 2, tr / 2)
    az = tz + rng.uniform(-tr / 2, tr / 2)
    emit(spheres, (ox, oy, oz, round(ax, 3), round(ay, 3), round(az, 3)))
