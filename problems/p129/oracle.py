import sys
import math

# Use float for geometry; input is integer but operations create floats
EPS = 1e-6

class Point:
    """2D point / vector with basic geometry operations."""
    __slots__ = ("x", "y")

    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    # Vector addition
    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    # Vector subtraction
    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    # Scalar multiplication
    def __mul__(self, c):
        if isinstance(c, (int, float)):
            return Point(self.x * c, self.y * c)
        # Dot product if multiplying by a Point: p * q
        return self.x * c.x + self.y * c.y

    # Allow scalar * Point as well
    def __rmul__(self, c):
        return self.__mul__(c)

    # Scalar division
    def __truediv__(self, c):
        return Point(self.x / c, self.y / c)

    # Cross product of two vectors (2D, returns scalar)
    def cross(self, other):
        return self.x * other.y - self.y * other.x

    # Squared length
    def norm2(self):
        return self.x * self.x + self.y * self.y

    # Length
    def norm(self):
        return math.sqrt(self.norm2())

    # Lexicographical comparison for sorting
    def __lt__(self, other):
        if abs(self.x - other.x) > EPS:
            return self.x < other.x
        return self.y < other.y

def ccw(a, b, c):
    """Orientation: >0 if a->b->c makes a left turn, <0 right, 0 collinear."""
    v = (b - a).cross(c - a)
    if -EPS <= v <= EPS:
        return 0
    return 1 if v > 0 else -1

def point_on_segment(a, b, p):
    """Check if point p lies on closed segment [a,b]."""
    if ccw(a, b, p) != 0:
        return False
    if p.x < min(a.x, b.x) - EPS or p.x > max(a.x, b.x) + EPS:
        return False
    if p.y < min(a.y, b.y) - EPS or p.y > max(a.y, b.y) + EPS:
        return False
    return True

def point_in_triangle(a, b, c, p):
    """Check if p lies inside or on edges of triangle abc."""
    d1 = ccw(a, b, p)
    d2 = ccw(b, c, p)
    d3 = ccw(c, a, p)
    return (d1 >= 0 and d2 >= 0 and d3 >= 0) or (d1 <= 0 and d2 <= 0 and d3 <= 0)

def line_line_intersection(a1, b1, a2, b2):
    """Intersection of infinite lines (a1,b1) and (a2,b2). They must not be parallel."""
    num = (a2 - a1).cross(b2 - a2)
    den = (b1 - a1).cross(b2 - a2)
    # t = num / den in parametrization a1 + t * (b1 - a1)
    t = num / den
    return a1 + (b1 - a1) * t

class ConvexHull:
    """Computes and stores a convex hull of a set of points."""

    def __init__(self, points):
        # Sort points lexicographically and remove duplicates
        pts = sorted(points, key=lambda p: (p.x, p.y))
        unique_pts = []
        for p in pts:
            if not unique_pts or abs(p.x - unique_pts[-1].x) > EPS or abs(p.y - unique_pts[-1].y) > EPS:
                unique_pts.append(p)
        self.points = unique_pts
        n = len(self.points)
        if n <= 2:
            # hull is the set itself
            return

        # Andrew's monotone chain variant using indices
        used = [False] * n
        hull = [0]

        def expand_hull(i, min_hull_size):
            # Pop while last turn is not strictly clockwise
            while len(hull) >= min_hull_size and \
                  ccw(self.points[hull[-2]], self.points[hull[-1]], self.points[i]) >= 0:
                used[hull[-1]] = False
                hull.pop()
            hull.append(i)
            used[i] = True

        # Lower hull
        for i in range(1, n):
            expand_hull(i, 2)

        upper_hull_size = len(hull)

        # Upper hull
        for i in range(n - 2, -1, -1):
            if not used[i]:
                expand_hull(i, upper_hull_size + 1)

        # Remove duplicate start/end index
        hull.pop()

        # Save hull points in order
        self.points = [self.points[i] for i in hull]

    def size(self):
        return len(self.points)

class PointInConvexPolygon:
    """Supports point-in-convex-polygon queries using fan triangulation around min point."""

    def __init__(self, polygon):
        self.polygon = polygon
        pts = polygon.points[:]
        # Find point with minimum (x,y)
        min_idx = 0
        for i in range(1, len(pts)):
            if pts[i].x < pts[min_idx].x - EPS or \
               (abs(pts[i].x - pts[min_idx].x) <= EPS and pts[i].y < pts[min_idx].y):
                min_idx = i
        self.min_point = pts[min_idx]
        # Remove it and sort the rest by angle
        del pts[min_idx]

        def cmp_angle(a, b):
            d = ccw(self.min_point, a, b)
            if d != 0:
                return -1 if d > 0 else 1
            # If collinear with min_point, closer one first
            if a.norm2() < b.norm2():
                return -1
            elif a.norm2() > b.norm2():
                return 1
            else:
                return 0

        # Python sort with key; need to simulate angle sort
        # Use atan2 as a key; ccw tie-break is not strictly necessary under no-3-collinear condition
        pts.sort(key=lambda p: math.atan2(p.y - self.min_point.y, p.x - self.min_point.x))
        self.points_by_angle = pts

    def contains(self, p):
        """Check if p is inside or on boundary of convex polygon."""
        pts = self.points_by_angle
        if len(pts) == 0:
            return False
        l, r = 0, len(pts) - 1
        # Binary search to find appropriate wedge (min_point, pts[l], pts[r])
        while r - l > 1:
            m = (l + r) // 2
            if ccw(self.min_point, pts[m], p) >= 0:
                l = m
            else:
                r = m
        return point_in_triangle(self.min_point, pts[l], pts[r], p)

def segments_intersect(a1, a2, b1, b2):
    """Check proper intersection between segments [a1,a2] and [b1,b2]."""
    d1 = ccw(b1, b2, a1)
    d2 = ccw(b1, b2, a2)
    d3 = ccw(a1, a2, b1)
    d4 = ccw(a1, a2, b2)
    return d1 * d2 < 0 and d3 * d4 < 0

def strictly_inside(p, hull, pip):
    """Check if p is strictly inside hull, not on boundary."""
    if not pip.contains(p):
        return False
    pts = hull.points
    k = len(pts)
    for i in range(k):
        p1 = pts[i]
        p2 = pts[(i + 1) % k]
        if point_on_segment(p1, p2, p):
            return False
    return True

def segment_length_inside(a, b, hull, pip):
    """Return length of part of segment [a,b] strictly inside hull."""
    points = [a, b]
    pts = hull.points
    k = len(pts)

    # Collect intersection/boundary points
    for i in range(k):
        p1 = pts[i]
        p2 = pts[(i + 1) % k]

        # Proper intersection
        if segments_intersect(a, b, p1, p2):
            inter = line_line_intersection(a, b, p1, p2)
            if point_on_segment(a, b, inter):
                points.append(inter)

        # Vertex on segment
        if point_on_segment(a, b, p1):
            points.append(p1)

        # Segment endpoint on edge
        if point_on_segment(p1, p2, a):
            points.append(a)
        if point_on_segment(p1, p2, b):
            points.append(b)

    # Sort points along the segment by parameter t
    ax, ay = a.x, a.y
    bx, by = b.x, b.y
    if abs(bx - ax) > EPS:
        def key_t(p):
            return (p.x - ax) / (bx - ax)
    else:
        def key_t(p):
            return (p.y - ay) / (by - ay)

    points.sort(key=key_t)

    # Deduplicate near-equal points
    unique_points = []
    for p in points:
        if not unique_points:
            unique_points.append(p)
        else:
            dx = p.x - unique_points[-1].x
            dy = p.y - unique_points[-1].y
            if dx * dx + dy * dy > EPS * EPS:
                unique_points.append(p)

    total = 0.0

    # Check each adjacent pair
    for i in range(len(unique_points) - 1):
        p1 = unique_points[i]
        p2 = unique_points[i + 1]
        mid = Point((p1.x + p2.x) * 0.5, (p1.y + p2.y) * 0.5)
        if pip.contains(p1) and pip.contains(p2) and strictly_inside(mid, hull, pip):
            dx = p2.x - p1.x
            dy = p2.y - p1.y
            total += math.hypot(dx, dy)

    return total

def main():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    n = int(next(it))
    vertices = []
    for _ in range(n):
        x = float(next(it))
        y = float(next(it))
        vertices.append(Point(x, y))

    hull = ConvexHull(vertices)
    pip = PointInConvexPolygon(hull)

    m = int(next(it))
    out_lines = []
    for _ in range(m):
        ax = float(next(it))
        ay = float(next(it))
        bx = float(next(it))
        by = float(next(it))
        a = Point(ax, ay)
        b = Point(bx, by)
        length = segment_length_inside(a, b, hull, pip)
        out_lines.append(f"{length:.2f}")
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()
