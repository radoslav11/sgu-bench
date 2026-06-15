import math
import sys

EPS = 1e-9
PI = math.acos(-1.0)

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, c):     return Point(self.x * c, self.y * c)
    def __truediv__(self, c): return Point(self.x / c, self.y / c)

    # Dot and cross
    def dot(self, other):   return self.x * other.x + self.y * other.y
    def cross(self, other): return self.x * other.y - self.y * other.x

    def norm2(self): return self.x * self.x + self.y * self.y
    def norm(self):  return math.hypot(self.x, self.y)

    def unit(self):
        n = self.norm()
        return self / n

    def perp(self):
        return Point(-self.y, self.x)

    def angle(self):
        return math.atan2(self.y, self.x)

def arc_area(center: Point, r: float, p1: Point, p2: Point) -> float:
    """
    Signed area contribution of the CCW arc from p1 to p2 on the circle
    (center, r), using the same closed-form integral as the C++ code.
    """
    t1 = (p1 - center).angle()
    t2 = (p2 - center).angle()
    if t2 < t1 - EPS:
        t2 += 2.0 * PI
    d = t2 - t1
    cx, cy = center.x, center.y
    area = r * cx * (math.sin(t2) - math.sin(t1)) \
         - r * cy * (math.cos(t2) - math.cos(t1)) \
         + r * r * d
    return 0.5 * area

def intersect_circles(c1: Point, r1: float, c2: Point, r2: float):
    """
    Return intersection points (0/1/2) of circles (c1,r1) and (c2,r2).
    Mirrors the C++ logic (reject coincident centers dist < EPS).
    """
    d = c2 - c1
    dist = d.norm()

    # No intersections (separate, contained, or coincident centers)
    if dist > r1 + r2 + EPS or dist < abs(r1 - r2) - EPS or dist < EPS:
        return []

    a = (r1*r1 - r2*r2 + dist*dist) / (2.0 * dist)
    h2 = r1*r1 - a*a
    if h2 < -EPS:
        return []
    if h2 < 0.0:
        h2 = 0.0
    h = math.sqrt(h2)

    mid = c1 + d.unit() * a
    perp_dir = d.perp().unit()

    if h < EPS:
        return [mid]
    return [mid + perp_dir * h, mid - perp_dir * h]

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    centers = []
    radii = []
    for _ in range(n):
        x = float(next(it)); y = float(next(it)); r = float(next(it))
        centers.append(Point(x, y))
        radii.append(r)

    # Collect events (angle, Point) for each circle
    events = [[] for _ in range(n)]

    for i in range(n):
        c = centers[i]
        r = radii[i]

        # Two arbitrary points to guarantee at least 2 events
        right = c + Point(r, 0.0)
        left  = c - Point(r, 0.0)
        events[i].append(((right - c).angle(), right))
        events[i].append(((left  - c).angle(), left))

        # All intersection points with other circles
        for j in range(n):
            if i == j:
                continue
            for p in intersect_circles(c, r, centers[j], radii[j]):
                events[i].append(((p - c).angle(), p))

        # Sort by angle
        events[i].sort(key=lambda t: t[0])

        # Deduplicate by angle (within EPS)
        uniq = []
        for ang, p in events[i]:
            if not uniq or abs(uniq[-1][0] - ang) > EPS:
                uniq.append((ang, p))
        events[i] = uniq

    def count_circles(p: Point) -> int:
        """Count circles whose interior strictly contains p."""
        cnt = 0
        for k in range(n):
            if (p - centers[k]).norm() < radii[k] - EPS:
                cnt += 1
        return cnt

    burned = 0.0
    inverse = 0.0

    # Process each circle's consecutive arcs
    for i in range(n):
        m = len(events[i])
        if m < 2:
            continue

        c = centers[i]
        r = radii[i]

        for j in range(m):
            ang1, p1 = events[i][j]
            ang2, p2 = events[i][(j + 1) % m]

            # Make ang2 >= ang1 by wrapping around
            if ang2 < ang1:
                ang2 += 2.0 * PI

            # Mid-angle direction to pick test points
            mid_ang = 0.5 * (ang1 + ang2)
            mid_dir = Point(math.cos(mid_ang), math.sin(mid_ang))

            # Slightly inside/outside to avoid boundary ambiguity
            test_inside = c + mid_dir * (r - 1e-6)
            test_outside = c + mid_dir * (r + 1e-6)

            cnt_in = count_circles(test_inside)
            cnt_out = count_circles(test_outside)

            piece = arc_area(c, r, p1, p2)

            # Add piece to the classification of the inside side
            if cnt_in % 2 == 1:
                burned += piece
            elif cnt_in > 0:
                inverse += piece

            # Subtract piece to the classification of the outside side
            if cnt_out % 2 == 1:
                burned -= piece
            elif cnt_out > 0:
                inverse -= piece

    # Print with enough precision
    print(f"{burned:.5f} {inverse:.5f}")

if __name__ == "__main__":
    solve()
