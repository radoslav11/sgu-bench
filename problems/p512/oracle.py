import sys
from bisect import bisect_left, bisect_right
from collections import defaultdict


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    pts = []
    idx = 1
    for i in range(n):
        x = int(data[idx]); y = int(data[idx + 1])
        idx += 2
        pts.append((x, y))

    groups = defaultdict(list)
    for x, y in pts:
        groups[x].append((x, y))
    pnts = [groups[x] for x in sorted(groups)]

    def dc(pnts):
        m = len(pnts)
        if m == 0:
            return 0
        if m == 1:
            return len(pnts[0]) - 1

        mid = m // 2
        left_groups = pnts[:mid]
        right_groups = pnts[mid:]

        left_flat = [pt for g in left_groups for pt in g]
        right_flat = [pt for g in right_groups for pt in g]

        ans = 0

        left_flat.sort(key=lambda p: (-p[1], p[0]))
        right_flat.sort(key=lambda p: (-p[1], p[0]))

        pareto_left = []
        pareto_right = []
        li = 0

        for rx, ry in right_flat:
            while pareto_right and pareto_right[-1][0] > rx:
                pareto_right.pop()
            while li < len(left_flat) and left_flat[li][1] >= ry:
                lx, ly = left_flat[li]
                while pareto_left and pareto_left[-1][0] <= lx:
                    pareto_left.pop()
                pareto_left.append((lx, ly))
                li += 1
            ans += len(pareto_left)
            if pareto_right:
                ry0 = pareto_right[-1][1]
                lo, hi = 0, len(pareto_left)
                while lo < hi:
                    mid2 = (lo + hi) // 2
                    if pareto_left[mid2][1] >= ry0:
                        lo = mid2 + 1
                    else:
                        hi = mid2
                ans -= lo
            pareto_right.append((rx, ry))

        left_flat.sort(key=lambda p: (p[1], p[0]))
        right_flat.sort(key=lambda p: (p[1], p[0]))

        pareto_left = []
        pareto_right = []
        li = 0

        for rx, ry in right_flat:
            while pareto_right and pareto_right[-1][0] > rx:
                pareto_right.pop()
            while li < len(left_flat) and left_flat[li][1] <= ry:
                lx, ly = left_flat[li]
                while pareto_left and pareto_left[-1][0] <= lx:
                    pareto_left.pop()
                pareto_left.append((lx, ly))
                li += 1

            if not pareto_right:
                ys = [p[1] for p in pareto_left]
                idx2 = bisect_left(ys, ry)
                ans += idx2
            else:
                ys = [p[1] for p in pareto_left]
                idx1 = bisect_left(ys, ry)
                y0 = pareto_right[-1][1]
                idx2 = bisect_right(ys, y0)
                ans += max(0, idx1 - idx2)

            pareto_right.append((rx, ry))

        return ans + dc(left_groups) + dc(right_groups)

    sys.setrecursionlimit(1 << 20)
    print(dc(pnts))


if __name__ == "__main__":
    main()
