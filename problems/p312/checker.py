"""Checker for p312: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any valid division is accepted. Piece i must be a triangle or
quadrilateral that has the kingdom side (V_i, V_{i+1}) as one of its
sides, must lie inside the kingdom, must not properly cross any other
piece, and must have area K_i / sum(K) of the kingdom's area. The pieces
are never compared against the reference's coordinates.
"""

import sys

VTX_TOL = 1e-3  # matching a kingdom vertex
DIST_TOL = 1e-3  # distance-based slack for containment / crossing tests


def reject(message):
    print(message)
    sys.exit(1)


def cross(o, a, b):
    return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])


def dist(a, b):
    return ((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2) ** 0.5


def signed_dist(a, b, p):
    # distance of p from line ab, signed; 0 for degenerate ab
    d = dist(a, b)
    if d < 1e-12:
        return 0.0
    return cross(a, b, p) / d


def point_seg_dist(p, a, b):
    ab2 = (b[0] - a[0]) ** 2 + (b[1] - a[1]) ** 2
    if ab2 < 1e-24:
        return dist(p, a)
    t = ((p[0] - a[0]) * (b[0] - a[0]) + (p[1] - a[1]) * (b[1] - a[1])) / ab2
    t = max(0.0, min(1.0, t))
    return dist(p, (a[0] + t * (b[0] - a[0]), a[1] + t * (b[1] - a[1])))


def proper_cross(p1, p2, p3, p4):
    d1 = signed_dist(p1, p2, p3)
    d2 = signed_dist(p1, p2, p4)
    d3 = signed_dist(p3, p4, p1)
    d4 = signed_dist(p3, p4, p2)
    t = DIST_TOL
    return ((d1 > t and d2 < -t) or (d1 < -t and d2 > t)) and (
        (d3 > t and d4 < -t) or (d3 < -t and d4 > t)
    )


def polygon_area(pts):
    s = 0.0
    for i in range(len(pts)):
        a, b = pts[i], pts[(i + 1) % len(pts)]
        s += a[0] * b[1] - a[1] * b[0]
    return abs(s) / 2.0


def in_polygon(p, poly):
    n = len(poly)
    for i in range(n):
        if point_seg_dist(p, poly[i], poly[(i + 1) % n]) <= DIST_TOL:
            return True
    inside = False
    x, y = p
    for i in range(n):
        (x1, y1), (x2, y2) = poly[i], poly[(i + 1) % n]
        if (y1 > y) != (y2 > y):
            xi = x1 + (y - y1) / (y2 - y1) * (x2 - x1)
            if xi > x:
                inside = not inside
    return inside


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    kingdom = [
        (float(in_tokens[1 + 2 * i]), float(in_tokens[2 + 2 * i]))
        for i in range(n)
    ]
    ks = [int(x) for x in in_tokens[1 + 2 * n].split(":")]
    total = polygon_area(kingdom)
    ksum = sum(ks)

    out_tokens = out_text.split()
    if not out_tokens:
        reject("empty output")
    if out_tokens[0] == "-1":
        if ans_tokens and ans_tokens[0] == "-1":
            sys.exit(0)
        reject("claimed impossible, but a division exists")

    pieces = []
    pos = 0
    for i in range(n):
        if pos >= len(out_tokens):
            reject(f"missing piece {i + 1}")
        try:
            cnt = int(out_tokens[pos])
        except ValueError:
            reject(f"piece {i + 1}: bad vertex count '{out_tokens[pos]}'")
        if cnt not in (3, 4):
            reject(f"piece {i + 1}: vertex count must be 3 or 4, got {cnt}")
        if pos + 1 + 2 * cnt > len(out_tokens):
            reject(f"piece {i + 1}: truncated coordinates")
        try:
            coords = [float(t) for t in out_tokens[pos + 1:pos + 1 + 2 * cnt]]
        except ValueError:
            reject(f"piece {i + 1}: non-numeric coordinate")
        pieces.append([(coords[2 * j], coords[2 * j + 1]) for j in range(cnt)])
        pos += 1 + 2 * cnt
    if pos != len(out_tokens):
        reject("trailing output after last piece")

    area_tol = max(1e-6, 2e-4 * total)
    for i, piece in enumerate(pieces):
        m = len(piece)
        # the kingdom side (V_i, V_{i+1}) must join neighbouring vertices
        a, b = kingdom[i], kingdom[(i + 1) % n]
        found = False
        for j in range(m):
            p, q = piece[j], piece[(j + 1) % m]
            if (dist(p, a) <= VTX_TOL and dist(q, b) <= VTX_TOL) or (
                dist(p, b) <= VTX_TOL and dist(q, a) <= VTX_TOL
            ):
                found = True
                break
        if not found:
            reject(f"piece {i + 1} does not have its kingdom side as a side")

        if m == 4:
            e = [(piece[j], piece[(j + 1) % 4]) for j in range(4)]
            if proper_cross(*e[0], *e[2]) or proper_cross(*e[1], *e[3]):
                reject(f"piece {i + 1} is self-intersecting")

        area = polygon_area(piece)
        want = ks[i] / ksum * total
        if abs(area - want) > area_tol:
            reject(
                f"piece {i + 1} area {area:.6f} differs from required "
                f"{want:.6f}"
            )

        for p in piece:
            if not in_polygon(p, kingdom):
                reject(f"piece {i + 1} vertex {p} lies outside the kingdom")
        for j in range(m):
            p, q = piece[j], piece[(j + 1) % m]
            for v in range(n):
                if proper_cross(p, q, kingdom[v], kingdom[(v + 1) % n]):
                    reject(f"piece {i + 1} edge crosses the kingdom border")

    for i in range(n):
        for j in range(i + 1, n):
            for a1 in range(len(pieces[i])):
                p, q = pieces[i][a1], pieces[i][(a1 + 1) % len(pieces[i])]
                for a2 in range(len(pieces[j])):
                    r, s = (
                        pieces[j][a2],
                        pieces[j][(a2 + 1) % len(pieces[j])],
                    )
                    if proper_cross(p, q, r, s):
                        reject(f"pieces {i + 1} and {j + 1} overlap")

    sys.exit(0)


if __name__ == "__main__":
    main()
