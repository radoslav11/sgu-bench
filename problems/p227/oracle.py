import sys
import math


# Floating-point tolerance.
EPS = 1e-9

# Pi and full circle angle.
PI = math.pi
FULL = 2.0 * PI


class Point:
    """2D point/vector with basic geometry operations."""

    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, value):
        # Multiplication by scalar.
        return Point(self.x * value, self.y * value)

    def __truediv__(self, value):
        return Point(self.x / value, self.y / value)

    def dot(self, other):
        return self.x * other.x + self.y * other.y

    def cross(self, other):
        return self.x * other.y - self.y * other.x

    def norm2(self):
        return self.x * self.x + self.y * self.y

    def norm(self):
        return math.sqrt(self.norm2())

    def unit(self):
        length = self.norm()
        return self / length

    def perp(self):
        return Point(-self.y, self.x)

    def angle(self):
        return math.atan2(self.y, self.x)


class Arc:
    """Processed representation of one input arc."""

    def __init__(self):
        self.is_segment = False
        self.a = None
        self.b = None
        self.c = None
        self.center = None
        self.r = 0.0


def ccw(a, b, c):
    """
    Orientation of triangle abc.
    Returns:
      1  if counterclockwise,
      -1 if clockwise,
      0  if collinear.
    """
    value = (b - a).cross(c - a)

    if -EPS <= value <= EPS:
        return 0

    return 1 if value > 0 else -1


def point_on_segment(a, b, p):
    """Check whether p lies on closed segment ab."""
    return (
        ccw(a, b, p) == 0
        and min(a.x, b.x) - EPS <= p.x <= max(a.x, b.x) + EPS
        and min(a.y, b.y) - EPS <= p.y <= max(a.y, b.y) + EPS
    )


def line_line_intersection(a1, b1, a2, b2):
    """Intersection point of two non-parallel infinite lines."""
    d1 = b1 - a1
    d2 = b2 - a2

    t = ((a2 - a1).cross(d2)) / d1.cross(d2)

    return a1 + d1 * t


def circumcenter(a, b, c):
    """Return center of the circle through non-collinear points a, b, c."""
    mid_ab = (a + b) / 2.0
    mid_ac = (a + c) / 2.0

    perp_ab = (b - a).perp()
    perp_ac = (c - a).perp()

    return line_line_intersection(mid_ab, mid_ab + perp_ab,
                                  mid_ac, mid_ac + perp_ac)


def norm_2pi(angle):
    """Normalize angle into [0, 2*pi)."""
    angle = math.fmod(angle, FULL)

    if angle < 0:
        angle += FULL

    return angle


def make_arc(p0, p1, p2):
    """Convert three input points into an Arc."""
    arc = Arc()

    arc.a = p0
    arc.b = p1
    arc.c = p2

    # Degenerate circular arc: treat as segment.
    if ccw(p0, p1, p2) == 0:
        arc.is_segment = True
    else:
        arc.is_segment = False
        arc.center = circumcenter(p0, p1, p2)
        arc.r = (p0 - arc.center).norm()

    return arc


def on_circle_arc(arc, p):
    """
    Check whether point p belongs to circular arc.
    Assumes p is on the supporting circle.
    """
    side_c = (arc.b - arc.a).cross(arc.c - arc.a)
    side_p = (arc.b - arc.a).cross(p - arc.a)

    # Points on chord line are endpoints/touching boundary points.
    if abs(side_p) < EPS:
        return True

    return (side_c > 0) == (side_p > 0)


def arc_interval(arc):
    """
    Represent an arc on its circle as a counterclockwise angular interval:
    (start_angle, length).
    """
    ang_a = (arc.a - arc.center).angle()
    ang_b = (arc.b - arc.center).angle()
    ang_c = (arc.c - arc.center).angle()

    span_b = norm_2pi(ang_b - ang_a)
    span_c = norm_2pi(ang_c - ang_a)

    # If c is encountered while going CCW from a to b,
    # then the intended arc is a -> b.
    if span_c <= span_b:
        return ang_a, span_b

    # Otherwise the intended arc is b -> a.
    return ang_b, FULL - span_b


def arc_overlap(x, y):
    """Return positive angular overlap length for same-circle arcs."""
    start_x, len_x = arc_interval(x)
    start_y, len_y = arc_interval(y)

    # Work in coordinate system where x starts at angle 0.
    shift = norm_2pi(start_y - start_x)

    overlap = 0.0

    def add(lo, hi):
        """Add overlap of interval [lo, hi] with [0, len_x]."""
        nonlocal overlap

        left = max(lo, 0.0)
        right = min(hi, len_x)

        if right > left:
            overlap += right - left

    # y interval does not wrap.
    if shift + len_y <= FULL:
        add(shift, shift + len_y)
    else:
        # y interval wraps around 2*pi.
        add(shift, FULL)
        add(0.0, shift + len_y - FULL)

    return overlap


def line_circle(a, b, center, r):
    """Intersect infinite line ab with circle."""
    direction = (b - a).unit()

    # Foot of perpendicular from center to line.
    foot = a + direction * ((center - a).dot(direction))

    dist = (center - foot).norm()

    if dist > r + EPS:
        return []

    h_sq = r * r - dist * dist

    # Clamp tiny negative caused by roundoff.
    if h_sq < 0:
        h_sq = 0.0

    h = math.sqrt(h_sq)

    if h < EPS:
        return [foot]

    return [foot + direction * h, foot - direction * h]


def intersect_circles(c1, r1, c2, r2):
    """Return intersection points of two circles."""
    d = c2 - c1
    dist = d.norm()

    # Separate, nested, or concentric.
    if dist > r1 + r2 + EPS or dist < abs(r1 - r2) - EPS or dist < EPS:
        return []

    a = (r1 * r1 - r2 * r2 + dist * dist) / (2.0 * dist)

    h_sq = r1 * r1 - a * a

    if h_sq < -EPS:
        return []

    if h_sq < 0:
        h_sq = 0.0

    h = math.sqrt(h_sq)

    mid = c1 + d.unit() * a
    perp_dir = d.perp().unit()

    if h < EPS:
        return [mid]

    return [mid + perp_dir * h, mid - perp_dir * h]


def intersect_arcs(x, y):
    """
    Intersect two processed arcs.
    Returns:
      infinite: bool
      points: list of finite crossing points
    """
    infinite = False
    points = []

    # Segment-segment case.
    if x.is_segment and y.is_segment:
        d1 = x.b - x.a
        d2 = y.b - y.a

        # Parallel.
        if abs(d1.cross(d2)) < EPS:
            # Different lines.
            if ccw(x.a, x.b, y.a) != 0:
                return False, []

            # Same line: project onto direction of first segment.
            u = d1.unit()
            length = (x.b - x.a).dot(u)

            pos_a = (y.a - x.a).dot(u)
            pos_b = (y.b - x.a).dot(u)

            lo = max(0.0, min(pos_a, pos_b))
            hi = min(length, max(pos_a, pos_b))

            if hi > lo + EPS:
                infinite = True
            elif hi > lo - EPS:
                points.append(x.a + u * ((lo + hi) / 2.0))

            return infinite, points

        # Non-parallel: one line intersection.
        p = line_line_intersection(x.a, x.b, y.a, y.b)

        if point_on_segment(x.a, x.b, p) and point_on_segment(y.a, y.b, p):
            points.append(p)

        return False, points

    # Segment vs circular arc.
    if x.is_segment != y.is_segment:
        seg = x if x.is_segment else y
        cir = y if x.is_segment else x

        for p in line_circle(seg.a, seg.b, cir.center, cir.r):
            if point_on_segment(seg.a, seg.b, p) and on_circle_arc(cir, p):
                points.append(p)

        return False, points

    # Circular arc vs circular arc on the same supporting circle.
    if (x.center - y.center).norm() < 1e-6 and abs(x.r - y.r) < 1e-6:
        if arc_overlap(x, y) > 1e-7:
            return True, []

        # If there is no positive overlap, only shared endpoints are possible.
        for p in [x.a, x.b, y.a, y.b]:
            if on_circle_arc(x, p) and on_circle_arc(y, p):
                points.append(p)

        return False, points

    # Circular arcs on different circles.
    for p in intersect_circles(x.center, x.r, y.center, y.r):
        if on_circle_arc(x, p) and on_circle_arc(y, p):
            points.append(p)

    return False, points


def solve():
    data = sys.stdin.read().strip().split()

    if not data:
        return

    it = iter(data)

    n = int(next(it))

    raw = []

    # Read triples of points.
    for _ in range(n):
        x1 = float(next(it))
        y1 = float(next(it))
        x2 = float(next(it))
        y2 = float(next(it))
        x3 = float(next(it))
        y3 = float(next(it))

        raw.append((Point(x1, y1), Point(x2, y2), Point(x3, y3)))

    # Build arcs.
    arcs = [make_arc(a, b, c) for a, b, c in raw]

    all_points = []
    has_infinite = False

    # Check all pairs.
    for i in range(n):
        for j in range(i + 1, n):
            infinite, points = intersect_arcs(arcs[i], arcs[j])

            if infinite:
                has_infinite = True

            all_points.extend(points)

    if has_infinite:
        print("Infinity")
        return

    # Deduplicate intersection points.
    unique = []

    for p in all_points:
        duplicate = False

        for q in unique:
            if (p - q).norm() < 1e-3:
                duplicate = True
                break

        if not duplicate:
            unique.append(p)

    # Sort by x, then y.
    unique.sort(key=lambda p: (p.x, p.y))

    print(len(unique))

    for p in unique:
        # Avoid printing -0.000.
        x = 0.0 if abs(p.x) < 5e-4 else p.x
        y = 0.0 if abs(p.y) < 5e-4 else p.y

        print(f"{x:.3f} {y:.3f}")


if __name__ == "__main__":
    solve()
