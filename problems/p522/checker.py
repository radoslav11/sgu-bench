"""Checker for p522 (Oil Wells): checker.py <in> <out> <ans>; exit 0 accept.

The minimum area is deterministic, so the output's first token must match
the reference. When a solution exists the second line is a route: a closed
rectilinear polyline of unit moves (W/N/E/S) that is simple (does not cross
or touch itself), starts and ends at the machine's initial position, uses
only two perpendicular directions before a single switch and the opposite
two after it, encloses every well (inside or on the border), and bounds
exactly the reported area. Any such route is accepted; the reference route
is never compared against.
"""

import sys

DIRS = {"N": (0, 1), "S": (0, -1), "E": (1, 0), "W": (-1, 0)}
# the two partitions of the four directions into perpendicular pairs
PARTITIONS = [({"N", "E"}, {"S", "W"}), ({"N", "W"}, {"S", "E"})]


def reject(msg):
    print(msg)
    sys.exit(1)


def main():
    with open(sys.argv[1]) as f:
        in_tokens = f.read().split()
    with open(sys.argv[2]) as f:
        out_tokens = f.read().split()
    with open(sys.argv[3]) as f:
        ans_tokens = f.read().split()

    n = int(in_tokens[0])
    x0 = int(in_tokens[1])
    y0 = int(in_tokens[2])
    wells = []
    pos = 3
    for _ in range(n):
        wx = int(in_tokens[pos]); wy = int(in_tokens[pos + 1])
        wells.append((wx, wy))
        pos += 2

    if not ans_tokens:
        reject("reference answer empty")
    expected_area = ans_tokens[0]

    if not out_tokens:
        reject("empty output")

    # -1 case
    if expected_area == "-1":
        if out_tokens[0] != "-1":
            reject(f"expected -1, got '{out_tokens[0]}'")
        sys.exit(0)

    if out_tokens[0] == "-1":
        reject("output claims -1 but a solution exists")

    try:
        out_area = int(out_tokens[0])
    except ValueError:
        reject(f"area is not an integer: '{out_tokens[0]}'")
    if out_area != int(expected_area):
        reject(f"area {out_area} != optimal {expected_area}")

    # the path is the second token (a string of moves)
    if len(out_tokens) < 2:
        reject("missing route")
    path = out_tokens[1]
    if len(out_tokens) > 2:
        reject("unexpected extra tokens after route")
    if len(path) < 4:
        reject("route too short for a non-degenerate polygon")
    for c in path:
        if c not in DIRS:
            reject(f"invalid move character '{c}'")

    # two-phase check: find a split into perpendicular pair then complement
    chars = set(path)
    valid_phase = False
    for g1, g2 in PARTITIONS:
        for first, second in ((g1, g2), (g2, g1)):
            # all of path must be coverable as prefix in `first`, suffix in `second`
            if not chars <= (first | second):
                continue
            # find first index that is not in `first`
            k = 0
            while k < len(path) and path[k] in first:
                k += 1
            if all(path[j] in second for j in range(k, len(path))):
                valid_phase = True
                break
        if valid_phase:
            break

    if not valid_phase:
        reject("route does not consist of two perpendicular phases")

    # trace vertices
    x, y = x0, y0
    verts = [(x, y)]
    edges = set()  # undirected unit edges as frozenset of endpoints
    for c in path:
        dx, dy = DIRS[c]
        nx, ny = x + dx, y + dy
        e = (min((x, y), (nx, ny)), max((x, y), (nx, ny)))
        if e in edges:
            reject("route reuses an edge (self-touching)")
        edges.add(e)
        x, y = nx, ny
        verts.append((x, y))

    if (x, y) != (x0, y0):
        reject("route is not closed")

    # all vertices except the closing one must be distinct (simple polygon)
    pts = verts[:-1]
    if len(set(pts)) != len(pts):
        reject("route visits a lattice vertex twice (self-touching)")

    # shoelace area (twice the area)
    area2 = 0
    for i in range(len(verts) - 1):
        x1, y1 = verts[i]
        x2, y2 = verts[i + 1]
        area2 += x1 * y2 - x2 * y1
    area = abs(area2) // 2
    if abs(area2) % 2 != 0:
        reject("non-integer polygon area")
    if area != out_area:
        reject(f"polygon area {area} != reported {out_area}")

    # point-in-polygon (inside or on border) for every well, via winding /
    # ray cast on the rectilinear polygon. Build boundary edge sets.
    # Collect all boundary lattice points and segments.
    seg_h = []  # horizontal segments (y, xlo, xhi)
    seg_v = []  # vertical segments (x, ylo, yhi)
    for i in range(len(verts) - 1):
        ax, ay = verts[i]
        bx, by = verts[i + 1]
        if ay == by:
            seg_h.append((ay, min(ax, bx), max(ax, bx)))
        else:
            seg_v.append((ax, min(ay, by), max(ay, by)))

    def on_border(px, py):
        for sy, lo, hi in seg_h:
            if py == sy and lo <= px <= hi:
                return True
        for sx, lo, hi in seg_v:
            if px == sx and lo <= py <= hi:
                return True
        return False

    def inside(px, py):
        if on_border(px, py):
            return True
        # ray cast to the right (+x). Count crossings with vertical segments.
        cnt = 0
        for sx, lo, hi in seg_v:
            if sx > px and lo <= py < hi:
                cnt += 1
        return cnt % 2 == 1

    for wx, wy in wells:
        if not inside(wx, wy):
            reject(f"well ({wx}, {wy}) is not enclosed")

    sys.exit(0)


if __name__ == "__main__":
    main()
