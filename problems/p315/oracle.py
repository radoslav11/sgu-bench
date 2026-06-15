import sys
import math
from collections import defaultdict

EPS = 1e-9
INF = 1e100

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, k):     return Point(self.x * k, self.y * k)

    def dot(self, other):   return self.x * other.x + self.y * other.y
    def cross(self, other): return self.x * other.y - self.y * other.x

    def norm2(self): return self.x * self.x + self.y * self.y
    def norm(self):  return math.hypot(self.x, self.y)
    def angle(self): return math.atan2(self.y, self.x)

def ccw(a: Point, b: Point, c: Point) -> int:
    """Orientation of triangle a-b-c with EPS tolerance."""
    v = (b - a).cross(c - a)
    if -EPS <= v <= EPS: return 0
    return 1 if v > 0 else -1

def point_on_segment(a: Point, b: Point, p: Point) -> bool:
    """Check if p lies on segment [a,b] with EPS tolerance."""
    if ccw(a, b, p) != 0:
        return False
    return (min(a.x, b.x) - EPS <= p.x <= max(a.x, b.x) + EPS and
            min(a.y, b.y) - EPS <= p.y <= max(a.y, b.y) + EPS)

def line_line_intersection(a1: Point, b1: Point, a2: Point, b2: Point) -> Point:
    """Intersection of infinite lines (a1-b1) and (a2-b2). Assumes not parallel."""
    d1 = b1 - a1
    d2 = b2 - a2
    # a1 + d1 * t, where t = cross(a2-a1, d2) / cross(d1, d2)
    t = (a2 - a1).cross(d2) / d1.cross(d2)
    return a1 + d1 * t

def crosses_up_ray(a: Point, b: Point) -> int:
    """
    Same test as in C++:
    count crossing of the half-line starting at origin going upward,
    implemented as crossing of x=0 with x from <=0 to >0 and y_at_cross > 0.
    """
    min_x = min(a.x, b.x)
    max_x = max(a.x, b.x)
    if not (min_x <= 0.0 and 0.0 < max_x):
        return 0
    # param t where x=0 on segment
    t = -a.x / (b.x - a.x)
    y = a.y + t * (b.y - a.y)
    return 1 if y > EPS else 0

def same_point(p: Point, q: Point) -> bool:
    return (p - q).norm() < EPS

def solve():
    it = iter(sys.stdin.read().strip().split())
    n = int(next(it))
    seg_a, seg_b = [], []
    for _ in range(n):
        x1 = float(next(it)); y1 = float(next(it))
        x2 = float(next(it)); y2 = float(next(it))
        seg_a.append(Point(x1, y1))
        seg_b.append(Point(x2, y2))

    origin = Point(0.0, 0.0)

    # Filter out segments that contain the origin
    seg_valid = [True] * n
    for i in range(n):
        if point_on_segment(seg_a[i], seg_b[i], origin):
            seg_valid[i] = False

    # Collect points on each segment: endpoints + intersections
    seg_pts = [[] for _ in range(n)]
    for i in range(n):
        if not seg_valid[i]:
            continue
        seg_pts[i].append(seg_a[i])
        seg_pts[i].append(seg_b[i])

    # Add intersection points / collinear endpoint inclusions
    for i in range(n):
        if not seg_valid[i]:
            continue
        for j in range(i + 1, n):
            if not seg_valid[j]:
                continue

            d1 = seg_b[i] - seg_a[i]
            d2 = seg_b[j] - seg_a[j]

            if abs(d1.cross(d2)) < EPS:
                # Parallel: only handle overlap by checking endpoints
                if point_on_segment(seg_a[i], seg_b[i], seg_a[j]):
                    seg_pts[i].append(seg_a[j])
                if point_on_segment(seg_a[i], seg_b[i], seg_b[j]):
                    seg_pts[i].append(seg_b[j])
                if point_on_segment(seg_a[j], seg_b[j], seg_a[i]):
                    seg_pts[j].append(seg_a[i])
                if point_on_segment(seg_a[j], seg_b[j], seg_b[i]):
                    seg_pts[j].append(seg_b[i])
            else:
                p = line_line_intersection(seg_a[i], seg_b[i], seg_a[j], seg_b[j])
                if point_on_segment(seg_a[i], seg_b[i], p) and point_on_segment(seg_a[j], seg_b[j], p):
                    seg_pts[i].append(p)
                    seg_pts[j].append(p)

    # Sort points on each segment and unique them
    for i in range(n):
        if not seg_valid[i]:
            continue
        a = seg_a[i]
        seg_pts[i].sort(key=lambda p: (p - a).norm2())
        uniq = []
        for p in seg_pts[i]:
            if not uniq or not same_point(uniq[-1], p):
                uniq.append(p)
        seg_pts[i] = uniq

    # Build global unique vertex list
    pts = []
    for i in range(n):
        for p in seg_pts[i]:
            pts.append(p)

    # Deduplicate globally: sort lexicographically then eps-unique
    pts.sort(key=lambda p: (p.x, p.y))
    uniq = []
    for p in pts:
        if not uniq or not same_point(uniq[-1], p):
            uniq.append(p)
    pts = uniq
    m = len(pts)

    if m == 0:
        print("0")
        return

    # Map point -> index via linear search with EPS (matches C++ approach).
    # For speed, we can also use rounded hashing, but keep it conceptually identical.
    def get_idx(p: Point) -> int:
        for i in range(m):
            if same_point(pts[i], p):
                return i
        return -1

    # Build adjacency with deduplication using a dict keyed by (u,v,cross) storing best length
    adj_best = [dict() for _ in range(m)]

    for i in range(n):
        if not seg_valid[i]:
            continue
        arr = seg_pts[i]
        for j in range(len(arr) - 1):
            u = get_idx(arr[j])
            v = get_idx(arr[j + 1])
            d = (arr[j] - arr[j + 1]).norm()
            if d < EPS:
                continue
            if abs(pts[u].angle() - pts[v].angle()) < EPS:
                continue
            cross = crosses_up_ray(pts[u], pts[v])

            # undirected: add both ways
            key_uv = (v, cross)
            key_vu = (u, cross)
            if key_uv not in adj_best[u] or adj_best[u][key_uv] < d:
                adj_best[u][key_uv] = d
            if key_vu not in adj_best[v] or adj_best[v][key_vu] < d:
                adj_best[v][key_vu] = d

    adj = [[] for _ in range(m)]
    for v in range(m):
        for (to, cross), dist in adj_best[v].items():
            adj[v].append((to, dist, cross))

    ans = 0.0

    # Try both angular directions
    for direction in (0, 1):
        order = list(range(m))
        order.sort(key=lambda i: (pts[i].angle(), pts[i].norm()) if direction == 0
                              else (-pts[i].angle(), pts[i].norm()))
        global_pos = [0] * m
        for idx, v in enumerate(order):
            global_pos[v] = idx

        for start in range(m):
            rel_pos = [0] * m
            for v in range(m):
                rel_pos[v] = (global_pos[v] - global_pos[start] + m) % m

            vertex_at = [0] * m
            for v in range(m):
                vertex_at[rel_pos[v]] = v

            # dp[pos][parity]
            dp0 = [-INF] * (m + 1)
            dp1 = [-INF] * (m + 1)
            dp0[0] = 0.0

            for i in range(m):
                v = vertex_at[i]

                for parity in (0, 1):
                    cur = dp0[i] if parity == 0 else dp1[i]
                    if cur < -1e50:
                        continue

                    for (u, dist, cross) in adj[v]:
                        pu = rel_pos[u]
                        if pu == 0:
                            pu = m
                        if pu > i:
                            np = parity ^ cross
                            val = cur + dist
                            if np == 0:
                                if val > dp0[pu]:
                                    dp0[pu] = val
                            else:
                                if val > dp1[pu]:
                                    dp1[pu] = val

            if dp1[m] > ans:
                ans = dp1[m]

    # If impossible to build positive length, answer stays 0
    print(f"{ans:.9f}")

if __name__ == "__main__":
    solve()
