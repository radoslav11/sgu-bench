import sys
import math

EPS = 1e-9

def cross(ax, ay, bx, by):
    return ax * by - ay * bx

def dot(ax, ay, bx, by):
    return ax * bx + ay * by

def angle(dx, dy):
    return math.atan2(dy, dx)

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    k = int(next(it))

    pies = []  # list of (area, cuttable)

    for _ in range(n):
        # Read rectangle corners and two candles (all ints)
        ax, ay = int(next(it)), int(next(it))
        bx, by = int(next(it)), int(next(it))
        cx, cy = int(next(it)), int(next(it))
        dx, dy = int(next(it)), int(next(it))
        xx, xy = int(next(it)), int(next(it))
        yx, yy = int(next(it)), int(next(it))

        # Center of rectangle: average of 4 corners
        ox = (ax + bx + cx + dx) / 4.0
        oy = (ay + by + cy + dy) / 4.0

        # Sort corners by polar angle around center to get consistent polygon order
        corners = [(ax, ay), (bx, by), (cx, cy), (dx, dy)]
        corners.sort(key=lambda p: angle(p[0] - ox, p[1] - oy))

        # Shoelace via cross sum
        s = 0.0
        for i in range(4):
            x1, y1 = corners[i]
            x2, y2 = corners[(i + 1) % 4]
            s += cross(x1, y1, x2, y2)
        area = abs(s) / 2.0

        # Vectors from center to candles
        px, py = xx - ox, xy - oy
        qx, qy = yx - ox, yy - oy

        # Cuttable iff:
        # - not collinear (cross != 0), OR
        # - collinear but opposite directions (dot < 0)
        c = cross(px, py, qx, qy)
        d = dot(px, py, qx, qy)
        cuttable = (abs(c) > EPS) or (d < -EPS)

        pies.append((area, cuttable))

    # Sort by area descending (max total area selection)
    pies.sort(key=lambda t: t[0], reverse=True)

    if k == 0:
        print(f"{0.0:.3f} {0.0:.3f}")
        return

    threshold = pies[k - 1][0]

    fixed_area = 0.0    # forced pies above threshold (optimist share)
    above = 0           # count of pies with area > threshold
    total_eq = 0        # count of pies with area == threshold
    cuttable_eq = 0     # among equals, how many cuttable

    for area, cut in pies:
        if area > threshold + EPS:
            above += 1
            if cut:
                fixed_area += area / 2.0
        elif abs(area - threshold) < EPS:
            total_eq += 1
            if cut:
                cuttable_eq += 1

    need = k - above
    non_cuttable_eq = total_eq - cuttable_eq

    # Best case: pick as many cuttable among the equals
    opt = fixed_area + min(need, cuttable_eq) * threshold / 2.0

    # Worst case: pick non-cuttable first; forced cuttable are what's left
    pes = fixed_area + max(0, need - non_cuttable_eq) * threshold / 2.0

    print(f"{pes:.3f} {opt:.3f}")

if __name__ == "__main__":
    solve()
