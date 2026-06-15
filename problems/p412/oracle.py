import sys
import math


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0

    def rd():
        nonlocal idx
        v = int(data[idx])
        idx += 1
        return v

    n = rd()
    m = rd()
    poly = []
    for _ in range(n):
        x = rd()
        y = rd()
        poly.append((float(x), float(y)))
    shelves = []
    for _ in range(m):
        ax = rd()
        ay = rd()
        bx = rd()
        by = rd()
        shelves.append(((float(ax), float(ay)), (float(bx), float(by))))

    TWO_PI = 2.0 * math.pi

    def ang(p):
        a = math.atan2(p[1], p[0])
        if a < 0:
            a += TWO_PI
        return a

    # Blocked angular intervals: each shelf, viewed from the origin, covers the
    # minor arc (< pi) between the angles of its endpoints. The sign of the
    # endpoint cross product picks the counter-clockwise start.
    raw = []
    for (a, b) in shelves:
        cross = a[0] * b[1] - a[1] * b[0]
        if abs(cross) < 1e-12:
            continue
        sa = ang(a)
        sb = ang(b)
        if cross > 0:
            lo, hi = sa, sb
        else:
            lo, hi = sb, sa
        if lo <= hi:
            raw.append((lo, hi))
        else:
            raw.append((lo, TWO_PI))
            raw.append((0.0, hi))

    # Merge the angular intervals into disjoint covered ranges.
    raw.sort()
    merged = []
    for lo, hi in raw:
        if merged and lo <= merged[-1][1] + 1e-12:
            merged[-1] = (merged[-1][0], max(merged[-1][1], hi))
        else:
            merged.append((lo, hi))

    # For an angle theta, find the boundary point hit by the ray from the
    # origin. The origin is strictly inside the convex polygon, so exactly one
    # edge is crossed; locate it by walking all edges (independent of any sort).
    edges = [(poly[i], poly[(i + 1) % n]) for i in range(n)]

    def hit_point(theta):
        dx = math.cos(theta)
        dy = math.sin(theta)
        best = None
        for (p, q) in edges:
            ex = q[0] - p[0]
            ey = q[1] - p[1]
            denom = dx * ey - dy * ex
            if abs(denom) < 1e-15:
                continue
            # s*d - u*e = p ; Cramer with D = -denom.
            D = -denom
            s = (-p[0] * ey + ex * p[1]) / D
            if s <= 1e-12:
                continue
            u = (dx * p[1] - dy * p[0]) / D
            if -1e-9 <= u <= 1 + 1e-9:
                cand = (dx * s, dy * s, s)
                if best is None or s < best[2]:
                    best = cand
        return (best[0], best[1])

    def dist(a, b):
        return math.hypot(a[0] - b[0], a[1] - b[1])

    # Length of boundary between the points hit at angles lo and hi, traversing
    # counter-clockwise (increasing angle). Walk over the polygon vertices whose
    # angle lies strictly between lo and hi.
    vert_ang = [(ang(poly[i]), poly[i]) for i in range(n)]

    def boundary_len(lo, hi):
        start = hit_point(lo)
        end = hit_point(hi)
        between = sorted(
            (a, p) for (a, p) in vert_ang if lo + 1e-12 < a < hi - 1e-12
        )
        total = 0.0
        prev = start
        for _, p in between:
            total += dist(prev, p)
            prev = p
        total += dist(prev, end)
        return total

    ans = 0.0
    for lo, hi in merged:
        ans += boundary_len(lo, hi)

    print("%.12f" % ans)


if __name__ == "__main__":
    main()
