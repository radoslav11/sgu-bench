import sys
from math import sqrt


def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    a = [int(x) for x in data[1 : 1 + n]]

    # Each side is attached to one of three "super-edges" X, Y, Z, either in
    # the forward or backward direction. The infimum area is the smallest
    # Heron-area triangle over the resulting signed sums (x, y, z). We search
    # all assignments with memoization on (index, x, y, z); identical partial
    # signed sums collapse so the state space stays small.
    INF = float("inf")
    memo = {}

    def area(x, y, z):
        x, y, z = abs(x), abs(y), abs(z)
        if x + y < z or y + z < x or z + x < y:
            return INF
        s = (x + y + z) / 2.0
        val = s * (s - x) * (s - y) * (s - z)
        if val < 0:
            val = 0.0
        return sqrt(val)

    def rec(i, x, y, z):
        if i == n:
            return area(x, y, z)
        key = (i, x, y, z)
        cached = memo.get(key)
        if cached is not None:
            return cached
        v = a[i]
        best = min(
            rec(i + 1, x + v, y, z),
            rec(i + 1, x, y + v, z),
            rec(i + 1, x, y, z + v),
            rec(i + 1, x - v, y, z),
            rec(i + 1, x, y - v, z),
            rec(i + 1, x, y, z - v),
        )
        memo[key] = best
        return best

    sys.setrecursionlimit(100000)
    ans = rec(0, 0, 0, 0)
    print("%.10f" % ans)


main()
