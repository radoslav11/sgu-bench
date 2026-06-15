import math
import heapq
import sys

# Same discretization constants as the C++ solution
UMIN, UMAX, DU = -1.5, 1.5, 0.018
PHIMIN, PHIMAX, DPHI = -15.0, 15.0, 0.028
PI = math.acos(-1.0)

def clamp(x, lo, hi):
    return lo if x < lo else hi if x > hi else x

def solve():
    data = list(map(float, sys.stdin.read().split()))
    xa, ya, za, xb, yb, zb = data

    # Convert to u = asinh(z)
    ua = math.asinh(za)
    ub = math.asinh(zb)

    # Principal angular difference between B and A
    dphi = math.atan2(yb, xb) - math.atan2(ya, xa)
    # Normalize to (-pi, pi]
    dphi = math.fmod(dphi + 3 * PI, 2 * PI) - PI

    # Grid sizes (+15 padding as in C++)
    NU = int((UMAX - UMIN) / DU) + 15
    NP = int((PHIMAX - PHIMIN) / DPHI) + 15

    # Map continuous u to nearest grid index, clamped
    def getiu(u):
        idx = int(round((u - UMIN) / DU))
        return clamp(idx, 0, NU - 1)

    # Map continuous phi to nearest grid index, clamped
    def getip(p):
        idx = int(round((p - PHIMIN) / DPHI))
        return clamp(idx, 0, NP - 1)

    # Dijkstra distance array (use a large number as infinity)
    INF = 1e100
    dist = [[INF] * NP for _ in range(NU)]

    # Start at (ua, phi=0)
    su = getiu(ua)
    sp = getip(0.0)
    dist[su][sp] = 0.0

    # Min-heap entries are (cost, i, j)
    pq = [(0.0, su, sp)]

    # 8-neighborhood
    dirs = [(-1, -1), (-1, 0), (-1, 1),
            (0, -1),          (0, 1),
            (1, -1),  (1, 0), (1, 1)]

    while pq:
        cost, i, j = heapq.heappop(pq)
        if cost != dist[i][j]:
            continue

        # Convert indices to coordinate values
        u0 = UMIN + i * DU
        p0 = PHIMIN + j * DPHI

        for di, dj in dirs:
            ni, nj = i + di, j + dj
            if not (0 <= ni < NU and 0 <= nj < NP):
                continue

            u1 = UMIN + ni * DU
            p1 = PHIMIN + nj * DPHI

            # Midpoint approximation for u in the metric
            um = 0.5 * (u0 + u1)

            du = u1 - u0
            dp = p1 - p0

            # ds = sqrt( cosh(2u) du^2 + cosh(u)^2 dphi^2 )
            ds = math.sqrt(math.cosh(2.0 * um) * du * du +
                           (math.cosh(um) ** 2) * dp * dp)

            nc = cost + ds
            if nc < dist[ni][nj]:
                dist[ni][nj] = nc
                heapq.heappush(pq, (nc, ni, nj))

    # Evaluate best distance near B, considering periodic copies in phi
    ans = INF
    tu = getiu(ub)

    for k in range(-5, 6):
        tp = dphi + 2 * PI * k
        basep = getip(tp)

        # 7x7 neighborhood around the nearest cell
        for di in range(-3, 4):
            for dj in range(-3, 4):
                ti, tj = tu + di, basep + dj
                if 0 <= ti < NU and 0 <= tj < NP:
                    ans = min(ans, dist[ti][tj])

    if ans > 1e50:
        ans = 0.0

    # Print with fixed precision
    print(f"{ans:.10f}")

if __name__ == "__main__":
    solve()
