import sys
import math

def main():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    n = int(next(it))
    vw = float(next(it))
    vc = float(next(it))
    pts = [(float(next(it)), float(next(it))) for _ in range(n)]

    # dp[i]: min time to reach boundary point i
    dp = [1e18] * n
    dp[0] = 0.0

    for i in range(n):
        xi, yi = pts[i]

        # 1) Walk along adjacent slope
        if i > 0:
            x0, y0 = pts[i-1]
            d = math.hypot(xi - x0, yi - y0)
            dp[i] = min(dp[i], dp[i-1] + d / vw)

        # 2) Tunnel in from any earlier j
        #    Stop when the terrain dips below yi.
        for j in range(i-1, -1, -1):
            xj, yj   = pts[j]
            xj1, yj1 = pts[j+1]
            lo, hi = min(yj, yj1), max(yj, yj1)

            if lo <= yi <= hi:
                # Compute intersection I on segment j->(j+1) at height yi
                if yj1 != yj:
                    slope = (xj1 - xj) / (yj1 - yj)
                    xI = xj + slope*(yi - yj)
                else:
                    xI = xj1
                # Walk from (xj,yj) to I
                d1 = math.hypot(xI - xj, yi - yj)
                # Tunnel from I to (xi, yi)
                d2 = abs(xi - xI)
                dp[i] = min(dp[i], dp[j] + d1/vw + d2/vc)

            # Once yj <= yi, no further mountains cover yi from the left
            if yj <= yi:
                break

        # 3) Tunnel out to any future k
        for k in range(i+1, n):
            xk, yk   = pts[k]
            xkm, ykm = pts[k-1]
            lo, hi = min(ykm, yk), max(ykm, yk)

            if lo <= yi <= hi:
                # Intersection I at height yi on segment (k-1)->k
                if yk != ykm:
                    slope = (xk - xkm) / (yk - ykm)
                    xI = xkm + slope*(yi - ykm)
                else:
                    xI = xkm
                # Walk from I to (xk, yk)
                d1 = math.hypot(xk - xI, yk - yi)
                # Tunnel from (xi, yi) to I
                d2 = abs(xI - xi)
                dp[k] = min(dp[k], dp[i] + d1/vw + d2/vc)

            # Once yk <= yi, no further coverage
            if yk <= yi:
                break

    # Print result for last point
    print(f"{dp[n-1]:.10f}")

if __name__ == "__main__":
    main()
