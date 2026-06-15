import math
from bisect import bisect_left

EPS = 1e-9
PI = math.acos(-1.0)

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, k):     return Point(self.x * k, self.y * k)
    def __truediv__(self, k): return Point(self.x / k, self.y / k)

    # dot and cross
    def dot(self, other): return self.x * other.x + self.y * other.y
    def cross(self, other): return self.x * other.y - self.y * other.x

    def norm2(self): return self.x * self.x + self.y * self.y
    def norm(self): return math.hypot(self.x, self.y)
    def angle(self): return math.atan2(self.y, self.x)

    def perp(self): return Point(-self.y, self.x)

    def unit(self):
        n = self.norm()
        return self / n

    def __lt__(self, other):
        # Lexicographic order for hull building
        return (self.x, self.y) < (other.x, other.y)

def ccw(a: Point, b: Point, c: Point) -> int:
    v = (b - a).cross(c - a)
    if -EPS <= v <= EPS:
        return 0
    return 1 if v > 0 else -1

def normalize_angle(x: float) -> float:
    # Map to [0, 2*pi)
    x %= (2.0 * PI)
    if x < 0:
        x += 2.0 * PI
    return x

class ConvexHull:
    """
    Builds a convex hull in CCW order.
    Also stores 'lower' and 'upper' chains + a split index to support tangent search.
    This mirrors the structure used in the C++ code.
    """
    def __init__(self, pts):
        pts = sorted(pts)
        # deduplicate (input has unique points, but still safe)
        uniq = []
        for p in pts:
            if not uniq or (p.x != uniq[-1].x or p.y != uniq[-1].y):
                uniq.append(p)
        pts = uniq
        self.points = pts

        if len(pts) <= 2:
            # trivial hull
            self.lower_end = len(pts) - 1
            self.lower = pts[:]
            self.upper = [pts[-1]] + ([pts[0]] if len(pts) > 1 else [])
            return

        hull = [0]
        used = [False] * len(pts)

        def expand(i, min_size):
            # Maintain convexity with ccw >= 0 pop (same criterion as C++ code)
            while len(hull) >= min_size and ccw(pts[hull[-2]], pts[hull[-1]], pts[i]) >= 0:
                used[hull[-1]] = False
                hull.pop()
            hull.append(i)
            used[i] = True

        # lower
        for i in range(1, len(pts)):
            expand(i, 2)
        uhs = len(hull)

        # upper
        for i in range(len(pts) - 2, -1, -1):
            if not used[i]:
                expand(i, uhs + 1)

        hull.pop()  # remove repeated start

        # Build final CCW point order (reverse like C++)
        final_pts = [pts[i] for i in hull][::-1]
        self.points = final_pts
        n = len(final_pts)

        self.lower_end = n - uhs
        self.lower = final_pts[:self.lower_end + 1]
        self.upper = final_pts[self.lower_end:] + [final_pts[0]]

    def __len__(self):
        return len(self.points)

    def __getitem__(self, i):
        return self.points[i]

    def tangents_from(self, p: Point):
        """
        Return indices (a,b) of tangent points from external point p to this hull.
        Port of the C++ method; relies on binary searching on lower/upper chains.
        """
        n = len(self.points)
        if n <= 1:
            return (0, 0)

        a = 0
        b = 0

        def update(idx):
            nonlocal a, b
            idx %= n
            if ccw(p, self.points[a], self.points[idx]) > 0:
                a = idx
            if ccw(p, self.points[b], self.points[idx]) < 0:
                b = idx

        def bin_search(low, high):
            if low >= high:
                return
            update(low)
            sl = ccw(p, self.points[low % n], self.points[(low + 1) % n])
            while low + 1 < high:
                mid = (low + high) // 2
                if ccw(p, self.points[mid % n], self.points[(mid + 1) % n]) == sl:
                    low = mid
                else:
                    high = mid
            update(high)

        # lower_bound on lower chain by lexicographic order
        lid = bisect_left([(q.x, q.y) for q in self.lower], (p.x, p.y))
        bin_search(0, lid)
        bin_search(lid, len(self.lower) - 1)

        # lower_bound on upper chain by "greater<Point>" order in C++:
        # i.e. decreasing lexicographic. We can bisect on keys (-x,-y).
        upper_keys = [(-q.x, -q.y) for q in self.upper]
        uid = bisect_left(upper_keys, (-p.x, -p.y))
        base = self.lower_end
        bin_search(base, base + uid)
        bin_search(base + uid, base + len(self.upper) - 1)

        return (a, b)

def cap_area(R: float, o: Point, alpha: float) -> float:
    """
    Area of circle cap determined by line through point o with direction alpha.
    Uses quadratic to find chord endpoints q1,q2 and formula:
      area = 1/2 * R^2 * dtheta + 1/2 * (q1 x q2)
    """
    dir = Point(math.cos(alpha), math.sin(alpha))

    b = o.dot(dir)
    c = o.norm2() - R * R
    D = b * b - c
    if D < -EPS:
        return 0.0
    if D < 0.0:
        D = 0.0
    sd = math.sqrt(D)

    q1 = o + dir * (-b - sd)
    q2 = o + dir * (-b + sd)

    dt = normalize_angle(math.atan2(q2.cross(q1), q2.dot(q1)))
    return 0.5 * R * R * dt + 0.5 * q1.cross(q2)

def solve_side(R: float, A: ConvexHull, B: ConvexHull) -> float:
    """
    Minimum possible cap area on A's side among valid separating lines.
    """
    ans = float("inf")
    n = len(A)

    edge_angle = []
    for i in range(n):
        edge = A[(i + 1) % n] - A[i]
        edge_angle.append(edge.angle())

    for i in range(n):
        p = A[i]

        events = []
        events.append((0.0, 0))  # ensure start event

        # stationary candidate directions
        if p.norm2() > EPS:
            a = p.perp().angle()
            events.append((normalize_angle(a), 0))
            events.append((normalize_angle(a + PI), 0))

        cnt = 0  # active ban count at angle 0

        def ban(l, r):
            nonlocal cnt
            l = normalize_angle(l + EPS)
            r = normalize_angle(r - EPS)
            if l > r:
                cnt += 1
            events.append((l, 1))
            events.append((r, -1))

        # forbid cutting A at p
        if n > 1:
            ban(edge_angle[i], edge_angle[(i + n - 1) % n] + 2.0 * PI)

        # forbid directions that would intersect B: use tangents from p to B
        ta, tb = B.tangents_from(p)
        angL = (B[tb] - p).angle() + PI
        angR = (B[ta] - p).angle()
        ban(angL, angR)

        events.sort()

        for ang, d in events:
            was = cnt
            cnt += d
            if was == 0 or cnt == 0:
                ans = min(ans, cap_area(R, p, ang))

    return ans

def main():
    import sys
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    R = float(next(it))

    n = int(next(it))
    ja = [Point(int(next(it)), int(next(it))) for _ in range(n)]

    m = int(next(it))
    se = [Point(int(next(it)), int(next(it))) for _ in range(m)]

    hja = ConvexHull(ja)
    hse = ConvexHull(se)

    min_area = solve_side(R, hja, hse)
    max_area = PI * R * R - solve_side(R, hse, hja)

    sys.stdout.write(f"{min_area:.15f} {max_area:.15f}\n")

if __name__ == "__main__":
    main()
