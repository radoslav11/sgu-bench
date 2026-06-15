import sys
import math

EPS = 1e-9
EPS_MERGE = 1e-12

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, k):     return Point(self.x * k, self.y * k)
    def __truediv__(self, k): return Point(self.x / k, self.y / k)

    # dot product
    def dot(self, other): return self.x * other.x + self.y * other.y
    # cross product (2D scalar)
    def cross(self, other): return self.x * other.y - self.y * other.x

    def norm2(self): return self.x * self.x + self.y * self.y
    def norm(self):  return math.hypot(self.x, self.y)

def rotate_cw(p: Point, k: int) -> Point:
    """Rotate point clockwise by 90 degrees k times."""
    k %= 4
    x, y = p.x, p.y
    for _ in range(k):
        x, y = y, -x
    return Point(x, y)

def segseg_intersection(a: Point, b: Point, c: Point, d: Point):
    """
    Segment-segment intersection:
      returns [] no intersection
      returns [P] intersection point
      returns [P,Q] overlapping segment endpoints (collinear overlap)
    """
    ab = b - a
    cd = d - c
    denom = ab.cross(cd)

    # Non-parallel: check if intersection parameters lie in [0,1]
    if abs(denom) > EPS:
        t = (c - a).cross(cd) / denom
        s = (c - a).cross(ab) / denom
        if -EPS <= t <= 1 + EPS and -EPS <= s <= 1 + EPS:
            return [a + ab * t]
        return []

    # Parallel: if not collinear => no intersection
    if abs((c - a).cross(ab)) > EPS:
        return []

    # Collinear: project c and d onto ab to get t-parameters
    len2 = ab.norm2()
    tc = (c - a).dot(ab) / len2
    td = (d - a).dot(ab) / len2
    if tc > td:
        tc, td = td, tc

    lo = max(0.0, tc)
    hi = min(1.0, td)
    if hi < lo - EPS:
        return []

    P = a + ab * lo
    Q = a + ab * hi
    if hi - lo < EPS:
        return [P]
    return [P, Q]

def merge_intervals(intervals):
    """Merge sorted intervals [lo,hi] with small tolerance."""
    intervals.sort()
    merged = []
    for lo, hi in intervals:
        if not merged or lo > merged[-1][1] + EPS_MERGE:
            merged.append([lo, hi])
        else:
            merged[-1][1] = max(merged[-1][1], hi)
    return merged

def in_intervals(t, intervals):
    """Check if t belongs to union of intervals (with EPS)."""
    for lo, hi in intervals:
        if t >= lo - EPS and t <= hi + EPS:
            return True
    return False

def solve(segs):
    total_length = 0.0
    total_integral = 0.0
    discrete_points = []

    for a, b in segs:
        ab = b - a
        L = ab.norm()
        if L < 1e-12:
            continue
        len2 = ab.norm2()

        # Build E[k] for k=1..3 (store at index 0..2)
        E = []
        for k in (1, 2, 3):
            raw = []
            for c, d in segs:
                c2 = rotate_cw(c, k)
                d2 = rotate_cw(d, k)
                inter = segseg_intersection(a, b, c2, d2)
                if not inter:
                    continue

                # map to t on [a,b]
                t1 = (inter[0] - a).dot(ab) / len2
                t1 = max(0.0, min(1.0, t1))
                if len(inter) == 1:
                    raw.append((t1, t1))
                else:
                    t2 = (inter[1] - a).dot(ab) / len2
                    t2 = max(0.0, min(1.0, t2))
                    if t1 > t2:
                        t1, t2 = t2, t1
                    raw.append((t1, t2))

            E.append(merge_intervals(raw))

        # Candidate t values are all endpoints + {0,1}
        ts = [0.0, 1.0]
        for k in range(3):
            for lo, hi in E[k]:
                ts.append(lo)
                ts.append(hi)
        ts.sort()

        # dedupe with tolerance
        cand = []
        for t in ts:
            t = max(0.0, min(1.0, t))
            if not cand or abs(t - cand[-1]) > EPS_MERGE:
                cand.append(t)

        # coefficients for |P(t)|^2 = qa t^2 + qb t + qc
        dx, dy = ab.x, ab.y
        qa = dx*dx + dy*dy
        qb = 2.0 * (a.x*dx + a.y*dy)
        qc = a.x*a.x + a.y*a.y

        # antiderivative of |P(t)|^2
        def F(t):
            return (qa/3.0)*t*t*t + (qb/2.0)*t*t + qc*t

        # integrate over feasible subintervals determined by midpoint test
        for i in range(len(cand) - 1):
            tlo, thi = cand[i], cand[i+1]
            if thi - tlo < EPS_MERGE:
                continue
            mid = 0.5 * (tlo + thi)
            if (in_intervals(mid, E[0]) and
                in_intervals(mid, E[1]) and
                in_intervals(mid, E[2])):

                total_length += (thi - tlo) * L
                total_integral += 2.0 * L * (F(thi) - F(tlo))

        # collect discrete feasible candidates (for 0D case)
        for t in cand:
            if (in_intervals(t, E[0]) and
                in_intervals(t, E[1]) and
                in_intervals(t, E[2])):
                discrete_points.append(a + ab * t)

    if total_length > 1e-9:
        return total_integral / total_length

    # If only points, dedupe and average 2|P|^2
    uniq = []
    for p in discrete_points:
        ok = True
        for q in uniq:
            if (p - q).norm() < 1e-6:
                ok = False
                break
        if ok:
            uniq.append(p)

    # guaranteed at least one solution
    s = 0.0
    for p in uniq:
        s += 2.0 * p.norm2()
    return s / len(uniq)

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    segs = []
    for _ in range(n):
        x1 = int(next(it)); y1 = int(next(it))
        x2 = int(next(it)); y2 = int(next(it))
        segs.append((Point(x1, y1), Point(x2, y2)))

    ans = solve(segs)
    print(f"{ans:.10f}")

if __name__ == "__main__":
    main()
