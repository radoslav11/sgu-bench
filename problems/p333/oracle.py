import math
from typing import Optional, Callable, List, Tuple

EPS = 1e-9
INF = 1e18

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x: float = 0.0, y: float = 0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other: "Point") -> "Point":
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other: "Point") -> "Point":
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, c: float) -> "Point":
        return Point(self.x * c, self.y * c)

    def __truediv__(self, c: float) -> "Point":
        return Point(self.x / c, self.y / c)

    def dot(self, other: "Point") -> float:
        return self.x * other.x + self.y * other.y

    def cross(self, other: "Point") -> float:
        return self.x * other.y - self.y * other.x

    def norm2(self) -> float:
        return self.x * self.x + self.y * self.y

    def norm(self) -> float:
        return math.hypot(self.x, self.y)

    def perp(self) -> "Point":
        return Point(-self.y, self.x)

    def unit(self) -> "Point":
        n = self.norm()
        return self / n

def intersect_ray_segment(ray_start: Point, ray_through: Point,
                          a: Point, b: Point) -> Optional[Point]:
    """
    Intersect ray (ray_start -> ray_through) with segment [a,b].
    Returns intersection point if:
      - intersection exists (not parallel)
      - ray parameter t > 0
      - segment parameter s in (0,1) with EPS margins (matching C++ code)
    """
    ray_dir = ray_through - ray_start
    if ray_dir.norm2() < EPS:
        return None
    seg_dir = b - a
    denom = ray_dir.cross(seg_dir)
    if abs(denom) < EPS:
        return None

    # Solve ray_start + t*ray_dir = a + s*seg_dir
    t = (a - ray_start).cross(seg_dir) / denom
    if t < EPS:
        return None
    s = (a - ray_start).cross(ray_dir) / denom
    if s < EPS or s > 1.0 - EPS:
        return None
    return ray_start + ray_dir * t

def adaptive_simpson(a: float, b: float, f: Callable[[float], float],
                     tol: float, int_whole: float = float("nan")) -> float:
    """
    Adaptive Simpson integral of f over [a,b].
    """
    if math.isnan(int_whole):
        m = 0.5 * (a + b)
        int_whole = (b - a) / 6.0 * (f(a) + 4.0 * f(m) + f(b))

    m = 0.5 * (a + b)
    fm = f(m)

    left_mid = 0.5 * (a + m)
    right_mid = 0.5 * (m + b)

    int_l = (m - a) / 6.0 * (f(a) + 4.0 * f(left_mid) + fm)
    int_r = (b - m) / 6.0 * (fm + 4.0 * f(right_mid) + f(b))

    if abs((int_l + int_r) - int_whole) < tol:
        # Richardson-like correction used in the C++ code
        return 0.5 * (int_l + int_r + int_whole)

    return (adaptive_simpson(a, m, f, tol, int_l) +
            adaptive_simpson(m, b, f, tol, int_r))

def solve() -> None:
    import sys
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    poly = [Point(float(next(it)), float(next(it))) for _ in range(n)]
    board = [Point(0, 0), Point(100, 0), Point(100, 100), Point(0, 100)]

    # We'll mimic the C++ globals via closures:
    now = Point(0, 0)

    def polar_less(a: Point, b: Point) -> bool:
        # Same inequality as C++:
        return (a.y - now.y) * (b.x - now.x) < (a.x - now.x) * (b.y - now.y)

    def compute_shadow_area(px: float, py: float) -> float:
        nonlocal now
        now = Point(px, py)
        shadow_poly: List[Point] = []

        # Find min and max vertex by "polar_less"
        min_idx = 0
        max_idx = 0
        for i in range(n):
            if polar_less(poly[i], poly[min_idx]):
                min_idx = i
            if polar_less(poly[max_idx], poly[i]):
                max_idx = i

        # Add chain from max_idx to min_idx (CCW)
        shadow_poly.append(poly[max_idx])
        j = (max_idx + 1) % n
        while j != min_idx:
            shadow_poly.append(poly[j])
            j = (j + 1) % n
        shadow_poly.append(poly[min_idx])

        # Intersect ray through min_idx with board boundary, find first hit
        start_i = -1
        thru_min = poly[min_idx]
        for i in range(4):
            a = board[i]
            b = board[(i + 1) % 4]
            inter = intersect_ray_segment(now, thru_min, a, b)
            if inter is not None:
                shadow_poly.append(inter)
                start_i = i
                break

            # Corner-on-ray special case (same as C++)
            cross_val = (a.x - now.x) * (thru_min.y - now.y) - (a.y - now.y) * (thru_min.x - now.x)
            dist = abs(a.x - now.x) + abs(a.y - now.y)
            if abs(cross_val) < EPS and dist > EPS:
                shadow_poly.append(a)
                start_i = i
                break

        if start_i == -1:
            return 0.0

        # Walk board edges adding corners until ray through max_idx hits
        thru_max = poly[max_idx]
        for jj in range(start_i, start_i + 4):
            idx = jj % 4
            if jj != start_i:
                shadow_poly.append(board[idx])

            a = board[idx]
            b = board[(idx + 1) % 4]
            inter = intersect_ray_segment(now, thru_max, a, b)
            if inter is not None:
                shadow_poly.append(inter)
                break

            cross_val = (board[idx].x - now.x) * (thru_max.y - now.y) - (board[idx].y - now.y) * (thru_max.x - now.x)
            dist = abs(board[idx].x - now.x) + abs(board[idx].y - now.y)
            if abs(cross_val) < EPS and dist > EPS:
                break

        # Area via triangulation from first vertex
        if len(shadow_poly) < 3:
            return 0.0
        area2 = 0.0
        p0 = shadow_poly[0]
        for i in range(1, len(shadow_poly) - 1):
            a = shadow_poly[i] - p0
            b = shadow_poly[i + 1] - p0
            area2 += a.cross(b)
        return 0.5 * area2

    def integrand_x(x: float) -> float:
        # Determine y-interval where point (x,y) lies inside the convex polygon
        ymin = INF
        ymax = -INF

        vert_start = Point(x, 0.0)
        vert_through = Point(x, 100.0)

        for i in range(n):
            a = poly[i]
            b = poly[(i + 1) % n]

            inter = intersect_ray_segment(vert_start, vert_through, a, b)
            if inter is not None:
                ymin = min(ymin, inter.y)
                ymax = max(ymax, inter.y)

            # Include vertices with x exactly equal (stabilization)
            if abs(poly[i].x - x) <= EPS:
                ymin = min(ymin, poly[i].y)
                ymax = max(ymax, poly[i].y)

        fy = lambda y: compute_shadow_area(x, y)

        # No intersection => always outside
        if ymin > ymax:
            return adaptive_simpson(0.0, 100.0, fy, 1e-4)

        # Inside strip => contribute full board area 10000 for those y
        low_b = ymin - EPS
        low = adaptive_simpson(0.0, low_b, fy, 1e-4) if low_b > 0.0 else 0.0

        high_a = ymax + EPS
        high = adaptive_simpson(high_a, 100.0, fy, 1e-4) if high_a < 100.0 else 0.0

        return (ymax - ymin) * 10000.0 + low + high

    # Outer integration over x
    integral = adaptive_simpson(0.0, 100.0, integrand_x, 1e-1)
    prob = integral / 1e8
    sys.stdout.write(f"{prob:.10f}\n")

if __name__ == "__main__":
    solve()
