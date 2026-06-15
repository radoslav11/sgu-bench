"""Tests for p308 (Hyperboloid Distance): two points on x^2+y^2-z^2=1.

Points are produced from the parametrization x = cosh(u) cos(phi),
y = cosh(u) sin(phi), z = sinh(u), with z in [-1, 1]. Seed 1 is the
sample-like easy case, seeds 2-3 are the hardest geometries (z at both
extremes with opposite angles, and exactly antipodal points across the
neck). Remaining seeds are random, including coincident points and
pairs forced to pass near the neck.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

UMAX = math.asinh(1.0)


def pt(u, phi):
    return (math.cosh(u) * math.cos(phi), math.cosh(u) * math.sin(phi), math.sinh(u))


if seed == 1:
    a, b = pt(0.0, 0.0), pt(0.0, math.pi / 2)
elif seed == 2:
    a, b = pt(-UMAX, 0.0), pt(UMAX, math.pi)
elif seed == 3:
    a, b = pt(UMAX, 1.2345), pt(UMAX, 1.2345 + math.pi)
elif seed == 4:
    u, phi = rng.uniform(-UMAX, UMAX), rng.uniform(-math.pi, math.pi)
    a = b = pt(u, phi)
elif seed == 5:
    a, b = pt(-UMAX, -2.5), pt(-UMAX, 2.9)
else:
    a = pt(rng.uniform(-UMAX, UMAX), rng.uniform(-math.pi, math.pi))
    b = pt(rng.uniform(-UMAX, UMAX), rng.uniform(-math.pi, math.pi))

print("%.12f %.12f %.12f" % a)
print("%.12f %.12f %.12f" % b)
