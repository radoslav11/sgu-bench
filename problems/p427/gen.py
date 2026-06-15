"""Tests for p427 (Hamiltonian polyhedron).

Every test is a nearly flat convex polyhedron: all vertices project onto a
strictly convex polygon (points of a circle), and heights are tiny, so the
cone at each vertex is a thin slab and the total solid angle stays far
below 4 steradians (bound: sum of 2D wedge angles * 2 * H / min_dist).

Seed 1 is the statement sample (minimal tetrahedron). Seed 2 is the
analytic "fan": 100 vertices, 100 facets, one of them a 99-gon (both the
vertex and the facet count at the limit). Seed 3 is a maximal simplicial
hull: 51 vertices -> 98 triangular facets. Other seeds are random hulls
and fans of various sizes.
"""

import math
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

SAMPLE = """4
3
100.146488845 0.000000000 0.000000000
100.145878719 0.349576483 0.000000000
-100.145878719 -0.349576483 0.000000000
3
33.382162948 0.000000000 1.219611194
100.146488845 0.000000000 0.000000000
100.145878719 0.349576483 0.000000000
3
33.382162948 0.000000000 1.219611194
100.145878719 0.349576483 0.000000000
-100.145878719 -0.349576483 0.000000000
3
33.382162948 0.000000000 1.219611194
-100.145878719 -0.349576483 0.000000000
100.146488845 0.000000000 0.000000000"""


def fmt(v):
    s = f"{v:.9f}"
    if float(s) == 0.0:
        s = "0.000000000"
    return s


def circle_points(n, radius, rng, height_max):
    # n sorted angles with a guaranteed gap, so chords stay large.
    min_gap = 0.8 * 2 * math.pi / n
    extra = [rng.random() for _ in range(n)]
    total = sum(extra)
    free = 2 * math.pi - n * min_gap
    angles = []
    acc = rng.random() * 2 * math.pi
    for e in extra:
        angles.append(acc)
        acc += min_gap + free * e / total
    pts = []
    for th in angles:
        z = rng.uniform(0.0, height_max)
        pts.append((radius * math.cos(th), radius * math.sin(th), z))
    return pts


def print_facets(pts, facets):
    strs = [(fmt(x), fmt(y), fmt(z)) for x, y, z in pts]
    print(len(facets))
    for f in facets:
        print(len(f))
        for v in f:
            print(" ".join(strs[v]))


def fan(n, rng):
    # v0 lifted, the rest at z = 0: one (n-1)-gon facet, one bottom
    # triangle, n-2 top triangles -> n facets, wheel graph.
    pts = circle_points(n, rng.uniform(200.0, 800.0), rng, 0.0)
    pts = [(x, y, 0.0) for x, y, _ in pts]
    x0, y0, _ = pts[0]
    pts[0] = (x0, y0, rng.uniform(0.005, 0.02) * 200.0 / 200.0)
    facets = [list(range(1, n))]
    facets.append([0, 1, n - 1])
    for i in range(1, n - 1):
        facets.append([0, i, i + 1])
    order = list(range(len(facets)))
    rng.shuffle(order)
    facets = [facets[i] for i in order]
    for f in facets:
        r = rng.randrange(len(f))
        f[:] = f[r:] + f[:r]
    print_facets(pts, facets)


def hull(n, rng):
    from scipy.spatial import ConvexHull

    pts = circle_points(n, rng.uniform(100.0, 300.0), rng, 0.02)
    h = ConvexHull(pts)
    assert len(h.vertices) == n, "not all points are hull vertices"
    facets = [list(s) for s in h.simplices]
    rng.shuffle(facets)
    print_facets(pts, facets)


if seed == 1:
    print(SAMPLE)
elif seed == 2:
    fan(100, rng)
elif seed == 3:
    hull(51, rng)
elif seed == 4:
    hull(4, rng)
elif seed in (5, 6):
    fan(rng.randint(5, 99), rng)
else:
    hull(rng.randint(5, 51), rng)
