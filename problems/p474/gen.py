"""Tests for p474 (All for Love): t <= 10 cases, each "n m" plus m
triangles (6 ints per line), n <= 25000, m <= 100000, total triangles
per file <= 200000, all coordinates in [0, 25000].

Valid coverages are produced by recursive 4-way midpoint splits of the
big triangle (children keep axis-parallel catheti), which also creates
T-junctions between leaves of different depth. NO cases are valid
coverages broken by shifting, duplicating or dropping a triangle, or by
injecting non-conforming triangles. Seeds 2-3 are the maximal files.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def vertices(t):
    rx, ry, dx, dy, s = t
    return [(rx, ry), (rx + s * dx, ry), (rx, ry + s * dy)]


def split(t):
    rx, ry, dx, dy, s = t
    h = s // 2
    return [
        (rx, ry, dx, dy, h),
        (rx + h * dx, ry, dx, dy, h),
        (rx, ry + h * dy, dx, dy, h),
        (rx + h * dx, ry + h * dy, -dx, -dy, h),
    ]


def refine_random(n, target):
    tris = [(0, 0, 1, 1, n)]
    guard = 0
    while len(tris) < target and guard < 50 * target:
        guard += 1
        i = rng.randrange(len(tris))
        if tris[i][4] >= 2 and tris[i][4] % 2 == 0:
            ch = split(tris[i])
            tris[i] = ch[0]
            tris.extend(ch[1:])
    return tris


def refine_uniform(n, depth):
    tris = [(0, 0, 1, 1, n)]
    for _ in range(depth):
        nxt = []
        for t in tris:
            nxt.extend(split(t))
        tris = nxt
    return tris


def to_points(tris):
    out = []
    for t in tris:
        vs = vertices(t)
        rng.shuffle(vs)
        out.append(vs)
    rng.shuffle(out)
    return out


def perturb(pts, kind):
    if kind == "hole":
        pts.pop(rng.randrange(len(pts)))
    elif kind == "shift":
        i = rng.randrange(len(pts))
        pts[i] = [(x + 1, y) for x, y in pts[i]]
    elif kind == "dup":
        # replace one triangle with a copy of an equal-area one
        by_area = {}
        for idx, tri in enumerate(pts):
            (x1, y1), (x2, y2), (x3, y3) = tri
            a = abs((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1))
            by_area.setdefault(a, []).append(idx)
        for a, idxs in by_area.items():
            if len(idxs) >= 2:
                i, j = rng.sample(idxs, 2)
                pts[i] = list(pts[j])
                break
    return pts


def case_lines(n, pts):
    lines = [f"{n} {len(pts)}"]
    for tri in pts:
        lines.append(" ".join(f"{x} {y}" for x, y in tri))
    return lines


cases = []

if seed == 1:
    cases.append((1, [[(0, 0), (0, 1), (1, 0)]]))
elif seed == 2:
    cases.append((16384, to_points(refine_random(16384, 100000))))
    bad = to_points(refine_random(16384, 99997))
    cases.append((16384, perturb(bad, "shift")))
elif seed == 3:
    cases.append((24576, to_points(refine_uniform(24576, 8))))
    cases.append((16384, perturb(to_points(refine_random(16384, 60000)), "dup")))
    cases.append((16384, perturb(to_points(refine_random(16384, 60000)), "hole")))
    cases.append((25000, to_points(refine_uniform(25000, 3))))
    cases.append((25000, [[(0, 0), (25000, 0), (0, 25000)]]))
elif seed == 4:
    # the three statement samples plus small handmade invalid shapes
    cases.append((1, [[(0, 0), (0, 1), (1, 0)]]))
    cases.append(
        (2, [[(0, 0), (1, 1), (0, 1)], [(0, 1), (1, 1), (0, 2)],
             [(1, 1), (2, 0), (1, 0)]])
    )
    cases.append(
        (3, [[(0, 0), (0, 1), (1, 1)], [(0, 0), (1, 0), (1, 1)],
             [(1, 0), (1, 1), (2, 1)], [(1, 0), (2, 0), (2, 1)],
             [(2, 0), (2, 1), (3, 0)], [(0, 1), (2, 1), (0, 3)]])
    )
    # right isosceles but diagonal catheti
    cases.append((2, [[(0, 0), (2, 0), (1, 1)], [(0, 0), (1, 1), (0, 2)],
                      [(1, 1), (2, 0), (2, 2)]]))
    # non-isosceles right triangle
    cases.append((2, [[(0, 0), (2, 0), (0, 1)], [(0, 1), (2, 0), (0, 2)]]))
    # triangle pokes outside the large one
    cases.append((2, [[(0, 0), (2, 0), (2, 2)], [(0, 0), (2, 2), (0, 2)]]))
    # correct area but overlapping copies
    cases.append((2, [[(0, 0), (2, 0), (0, 2)], [(0, 0), (1, 0), (0, 1)],
                      [(0, 0), (1, 0), (0, 1)]]))
else:
    # random batches of small/medium cases, even split YES / broken
    t = rng.randint(5, 10)
    sizes = [rng.choice([4, 8, 16, 32, 64, 128, 256]) for _ in range(t)]
    for n in sizes:
        target = min(rng.randint(2, 400), n * n)
        pts = to_points(refine_random(n, target))
        kind = rng.choice(["ok", "ok", "hole", "shift", "dup"])
        if kind != "ok":
            pts = perturb(pts, kind)
        cases.append((n, pts))

out = [str(len(cases))]
for n, pts in cases:
    out.extend(case_lines(n, pts))
sys.stdout.write("\n".join(out) + "\n")
