import sys
from typing import List, Tuple, Optional

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    p: List[Tuple[int, int]] = [(int(next(it)), int(next(it))) for _ in range(n)]

    # If there are no segments, track length is 0.
    if n <= 1:
        print(0)
        return

    m = n - 1  # number of segments

    # Segment length: since segments are axis-aligned, Manhattan distance equals segment length.
    def seg_len(i: int) -> int:
        x1, y1 = p[i]
        x2, y2 = p[i + 1]
        return abs(x2 - x1) + abs(y2 - y1)

    # Prefix lengths: prefix_len[k] = distance from start to vertex k
    prefix_len = [0] * (m + 1)
    for i in range(m):
        prefix_len[i + 1] = prefix_len[i] + seg_len(i)

    # Return intersection point of segment i and j, or None if no intersection.
    # Only handles proper crossing: one horizontal and one vertical.
    def get_intersection(i: int, j: int) -> Optional[Tuple[int, int]]:
        x1, y1 = p[i]
        x2, y2 = p[i + 1]
        x3, y3 = p[j]
        x4, y4 = p[j + 1]

        h1 = (y1 == y2)  # segment i horizontal?
        h2 = (y3 == y4)  # segment j horizontal?

        # Same orientation => ignore (no proper crossing; overlaps are disallowed).
        if h1 == h2:
            return None

        # Ensure i is horizontal, j is vertical (swap if needed).
        if not h1:
            i, j = j, i
            x1, y1 = p[i]
            x2, y2 = p[i + 1]
            x3, y3 = p[j]
            x4, y4 = p[j + 1]

        # Horizontal segment i spans x in [hx1, hx2] at y=hy
        hx1, hx2 = (x1, x2) if x1 <= x2 else (x2, x1)
        hy = y1

        # Vertical segment j spans y in [vy1, vy2] at x=vx
        vx = x3
        vy1, vy2 = (y3, y4) if y3 <= y4 else (y4, y3)

        # Check if they cross (including touching at endpoints)
        if hx1 <= vx <= hx2 and vy1 <= hy <= vy2:
            return (vx, hy)

        return None

    # Distance along the polyline to reach (px,py) while on segment i
    def pos_on_track(i: int, px: int, py: int) -> int:
        x, y = p[i]
        # Distance to segment start vertex + distance within segment
        return prefix_len[i] + abs(px - x) + abs(py - y)

    # Start with total track length (valid if no intersections exist)
    ans = prefix_len[m]

    # Check all non-adjacent pairs of segments:
    # j starts at i+2 to avoid adjacent segments sharing a vertex.
    for i in range(m):
        for j in range(i + 2, m):
            inter = get_intersection(i, j)
            if inter is None:
                continue
            px, py = inter

            # Arc-length positions of the same geometric point along the path
            pos_i = pos_on_track(i, px, py)
            pos_j = pos_on_track(j, px, py)

            # The loop length between the two occurrences
            cycle_len = pos_j - pos_i
            if cycle_len < ans:
                ans = cycle_len

    print(ans)

if __name__ == "__main__":
    solve()
