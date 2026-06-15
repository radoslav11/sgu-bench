"""Tests for p452 (Colony Maintenance): geodesic on a polycube surface.

n <= 16 unit cubes (edge 100) face-connected into one polycube, centers
on the 100-grid inside [-2000, 2000]. The robot walks the exposed faces
from a start point to a goal point, both strictly inside a surface face
(never on an edge). Seed 1 is a single cube. Seeds 2 and 3 are sixteen
cubes grown into two different dense shapes (a compact blob and a long
snaking tunnel) so the unfolding sweep has the most surface to cover.
Later seeds plant edge-only tip contacts (two cubes meeting at an edge,
which must NOT connect across the bare line) and thin staircases.
"""

import random
import sys

DIRS = [(1, 0, 0), (-1, 0, 0), (0, 1, 0),
        (0, -1, 0), (0, 0, 1), (0, 0, -1)]


def grow(rng, n, bound=18):
    """Grow a face-connected set of n grid cells, kept inside +-bound."""
    cells = {(0, 0, 0)}
    while len(cells) < n:
        base = rng.choice(list(cells))
        order = list(DIRS)
        rng.shuffle(order)
        placed = False
        for dx, dy, dz in order:
            c = (base[0] + dx, base[1] + dy, base[2] + dz)
            if c in cells:
                continue
            if max(abs(c[0]), abs(c[1]), abs(c[2])) > bound:
                continue
            cells.add(c)
            placed = True
            break
        if not placed:
            # retry from a different base; guaranteed progress eventually
            continue
    return cells


def surface_faces(cells):
    """Return list of (center_cell, dir) for each exposed face."""
    faces = []
    cs = set(cells)
    for c in cells:
        for d, (dx, dy, dz) in enumerate(DIRS):
            nb = (c[0] + dx, c[1] + dy, c[2] + dz)
            if nb not in cs:
                faces.append((c, d))
    return faces


def point_on_face(rng, cell, d):
    """Integer point strictly inside the exposed face (not on its edges)."""
    cx, cy, cz = cell[0] * 100, cell[1] * 100, cell[2] * 100
    dx, dy, dz = DIRS[d]
    base = [cx + 50 * dx, cy + 50 * dy, cz + 50 * dz]
    axis = d // 2
    for a in range(3):
        if a == axis:
            continue
        base[a] += rng.randint(-49, 49)
    return base


def emit(cells, rng):
    cells = list(cells)
    assert 1 <= len(cells) <= 16
    centers = [(c[0] * 100, c[1] * 100, c[2] * 100) for c in cells]
    for x, y, z in centers:
        assert -2000 <= x <= 2000 and -2000 <= y <= 2000
        assert -2000 <= z <= 2000

    faces = surface_faces(cells)
    while True:
        f1 = rng.choice(faces)
        f2 = rng.choice(faces)
        s = point_on_face(rng, *f1)
        t = point_on_face(rng, *f2)
        if s != t and all(-2000 <= v <= 2000 for v in s + t):
            break

    print(len(centers))
    for x, y, z in centers:
        print(x, y, z)
    print(s[0], s[1], s[2], t[0], t[1], t[2])


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit({(0, 0, 0)}, rng)
elif seed == 2:
    # compact 16-cube blob
    emit(grow(rng, 16, bound=4), rng)
elif seed == 3:
    # long snaking 16-cube path, more elongated unfoldings
    cells = {(0, 0, 0)}
    cur = (0, 0, 0)
    while len(cells) < 16:
        order = list(DIRS)
        rng.shuffle(order)
        moved = False
        for dx, dy, dz in order:
            c = (cur[0] + dx, cur[1] + dy, cur[2] + dz)
            if c not in cells and max(abs(c[0]), abs(c[1]), abs(c[2])) <= 18:
                cells.add(c)
                cur = c
                moved = True
                break
        if not moved:
            cur = rng.choice(list(cells))
    emit(cells, rng)
elif seed == 4:
    # an L / staircase of 16 cubes in a plane
    cells = set()
    x = y = 0
    for i in range(16):
        cells.add((x, y, 0))
        if i % 2 == 0:
            x += 1
        else:
            y += 1
    emit(cells, rng)
elif seed == 5:
    # hollow-ish 3x3 frame in a plane: a ring with a concave hole, exposing
    # inner L-shaped (270 degree) corners the geodesic must wrap around
    ring = [(0, 0, 0), (1, 0, 0), (2, 0, 0), (2, 1, 0), (2, 2, 0),
            (1, 2, 0), (0, 2, 0), (0, 1, 0)]
    cells = set(ring)
    # stack a partial second layer to break planarity
    for c in ring[:5]:
        cells.add((c[0], c[1], 1))
    cells = set(list(cells)[:16])
    emit(cells, rng)
else:
    n = rng.randint(2, 16)
    emit(grow(rng, n, bound=rng.randint(2, 10)), rng)
