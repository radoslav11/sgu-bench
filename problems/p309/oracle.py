import sys
from typing import List, Tuple

# We'll implement the same structure: check_k2, check_k3, and binary search.
# (check_k1 is not needed for this particular implementation, but included
# for completeness / parity with the C++ idea.)

def check_k1(points: List[Tuple[int, int]], idxs: List[int], d: int) -> bool:
    """
    Can the indexed points be covered by ONE d x d square?
    Use doubled-center intersection method to avoid fractions.
    """
    if not idxs:
        return True

    left, right = -2_000_000_000, 2_000_000_000   # feasible range for 2*cx
    bottom, top = -2_000_000_000, 2_000_000_000   # feasible range for 2*cy

    for i in idxs:
        x, y = points[i]
        left = max(left, 2 * x - d)
        right = min(right, 2 * x + d)
        bottom = max(bottom, 2 * y - d)
        top = min(top, 2 * y + d)

    return right >= left and top >= bottom


def check_k2(points: List[Tuple[int, int]], idxs: List[int], d: int) -> bool:
    """
    Can the indexed points be covered by TWO d x d squares?
    Try the two diagonal-corner configurations w.r.t. bounding box.
    """
    if not idxs:
        return True

    # Bounding box
    min_x = min(points[i][0] for i in idxs)
    max_x = max(points[i][0] for i in idxs)
    min_y = min(points[i][1] for i in idxs)
    max_y = max(points[i][1] for i in idxs)

    # Diagonal 1: bottom-left + top-right
    ok = True
    for i in idxs:
        x, y = points[i]
        in_bl = (x <= min_x + d and y <= min_y + d)
        in_tr = (x >= max_x - d and y >= max_y - d)
        if not (in_bl or in_tr):
            ok = False
            break
    if ok:
        return True

    # Diagonal 2: bottom-right + top-left
    ok = True
    for i in idxs:
        x, y = points[i]
        in_br = (x >= max_x - d and y <= min_y + d)
        in_tl = (x <= min_x + d and y >= max_y - d)
        if not (in_br or in_tl):
            ok = False
            break

    return ok


def check_k3(points: List[Tuple[int, int]], idxs: List[int], d: int) -> bool:
    """
    Can the indexed points be covered by THREE d x d squares?
    Try placing one square at each corner of the bounding box, then reduce to k=2.
    """
    if not idxs:
        return True

    # Bounding box
    min_x = min(points[i][0] for i in idxs)
    max_x = max(points[i][0] for i in idxs)
    min_y = min(points[i][1] for i in idxs)
    max_y = max(points[i][1] for i in idxs)

    def try_corner(cx: int, cy: int) -> bool:
        # Collect points not covered by square [cx,cx+d] x [cy,cy+d]
        rem = []
        for i in idxs:
            x, y = points[i]
            if x < cx or x > cx + d or y < cy or y > cy + d:
                rem.append(i)
        return check_k2(points, rem, d)

    # Try the four corners (adjusting by d to keep square aligned with bbox extremes)
    return (
        try_corner(min_x, min_y) or
        try_corner(max_x - d, min_y) or
        try_corner(min_x, max_y - d) or
        try_corner(max_x - d, max_y - d)
    )


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    points = []
    it = iter(data[1:])
    for xs, ys in zip(it, it):
        points.append((int(xs), int(ys)))

    idxs = list(range(n))

    # Binary search on d (same upper bound idea as C++)
    lo, hi = 0, 2_000_000_042
    ans = hi
    while lo <= hi:
        mid = (lo + hi) // 2
        if check_k3(points, idxs, mid):
            ans = mid
            hi = mid - 1
        else:
            lo = mid + 1

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    solve()
