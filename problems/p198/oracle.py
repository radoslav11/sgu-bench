
import sys
import math

def can_escape(circles, ship):
    # circles: list of (x, y, r)
    # ship: (cx, cy, cr)
    cx, cy, cr = ship
    n = len(circles)

    # Shift centers so the ship's center is at the origin and
    # inflate radii by the ship radius (Minkowski sum).
    x = [0.0] * n
    y = [0.0] * n
    R = [0.0] * n
    for i, (xi, yi, ri) in enumerate(circles):
        x[i] = xi - cx
        y[i] = yi - cy
        R[i] = ri + cr

    # Prepare angle for each center using atan2 (robust for all quadrants).
    # If a center is (0, 0) and R[i] > 0, that would imply the origin is inside a disc,
    # which contradicts "ship starts in free area". We keep the code robust anyway.
    theta = [math.atan2(y[i], x[i]) % (2.0 * math.pi) for i in range(n)]

    # Build directed graph weights:
    # w[i][j] = minimal signed angle to rotate from theta[i] to theta[j] if discs i and j intersect, else +inf.
    INF = 1e100
    w = [[INF] * n for _ in range(n)]
    for i in range(n):
        w[i][i] = 0.0

    # Epsilon used both in intersection tests and relaxations
    EPS = 1e-6
    for i in range(n):
        xi, yi, ri = x[i], y[i], R[i]
        for j in range(i + 1, n):
            xj, yj, rj = x[j], y[j], R[j]
            # Squared distance between centers
            dx = xi - xj
            dy = yi - yj
            center_dist_sq = dx * dx + dy * dy
            sum_rad = ri + rj
            sum_rad_sq = sum_rad * sum_rad

            # If they intersect/touch: center_dist <= sum_rad (+tiny tolerance).
            # This check mirrors the C++ logic with squared distances and a small EPS.
            if center_dist_sq + EPS <= sum_rad_sq:
                # Compute minimal signed angle difference in (−π, π]
                diff = theta[j] - theta[i]
                # Normalize into (−π, π]
                diff = (diff + math.pi) % (2.0 * math.pi) - math.pi
                w[i][j] = diff
                w[j][i] = -diff
            else:
                # Not intersecting: leave as INF
                pass

    # Floyd–Warshall to find all-pairs shortest paths in the angle graph.
    # If a cycle wraps around the origin clockwise, there exists k with w[k][k] < −π after closure.
    for k in range(n):
        wk = w[k]
        for i in range(n):
            wik = w[i][k]
            if wik >= INF:
                continue
            wi = w[i]
            # Relax i->j via k
            t = wik  # w[i][k]
            for j in range(n):
                val = t + wk[j] + EPS
                if val < wi[j]:
                    wi[j] = val

    # Detect a sufficiently negative cycle (enclosing the origin)
    for i in range(n):
        if w[i][i] < -math.pi:
            return False  # cannot escape

    return True  # can escape


def main():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    try:
        n = int(next(it))
    except StopIteration:
        return
    circles = []
    for _ in range(n):
        # Read xi, yi, ri as floats
        xi = float(next(it)); yi = float(next(it)); ri = float(next(it))
        circles.append((xi, yi, ri))
    cx = float(next(it)); cy = float(next(it)); cr = float(next(it))
    print("YES" if can_escape(circles, (cx, cy, cr)) else "NO")

if __name__ == "__main__":
    main()
