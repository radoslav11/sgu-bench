import sys
import math
from dataclasses import dataclass

# Numerical tolerances.
EPS = 1e-12
PI = math.acos(-1.0)


@dataclass
class Point:
    # 2D point/vector.
    x: float = 0.0
    y: float = 0.0

    def __add__(self, other):
        # Vector addition.
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        # Vector subtraction.
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, c):
        # Scalar multiplication.
        return Point(self.x * c, self.y * c)

    def __truediv__(self, c):
        # Scalar division.
        return Point(self.x / c, self.y / c)

    def dot(self, other):
        # Dot product.
        return self.x * other.x + self.y * other.y

    def norm2(self):
        # Squared length.
        return self.x * self.x + self.y * self.y

    def norm(self):
        # Euclidean length.
        return math.sqrt(self.norm2())


@dataclass
class DirResult:
    # Chosen descent direction and circle to follow.
    dir: Point
    arc_circle: int


@dataclass
class Segment:
    # A segment is either vertical or a circular arc.
    is_vert: bool

    # Vertical segment data.
    x: float = 0.0
    y0: float = 0.0
    y1: float = 0.0

    # Arc data.
    cx: float = 0.0
    r: float = 0.0
    theta0: float = 0.0
    theta1: float = 0.0

    # Euclidean length.
    length: float = 0.0


@dataclass
class Phase:
    # One motion phase after a cut.
    start_t: float
    total_len: float
    start_p: Point
    end_p: Point
    segs: list


def inside_disks(p, S, xs, ls, tol=1e-7):
    # Check whether point p lies inside every disk indexed by S.
    for i in S:
        dx = p.x - xs[i]
        dy = p.y
        if dx * dx + dy * dy > ls[i] * ls[i] + tol:
            return False
    return True


def circle_pair_inters(i, j, xs, ls):
    # Compute intersection points of circles i and j.
    dx = xs[j] - xs[i]
    d = abs(dx)

    # Same center: no useful pair intersections for this solution.
    if d < 1e-14:
        return []

    r1 = ls[i]
    r2 = ls[j]

    # No intersection if circles are disjoint or one is inside the other.
    if d > r1 + r2 + 1e-12 or d < abs(r1 - r2) - 1e-12:
        return []

    # Distance from center i to chord midpoint.
    a = (r1 * r1 - r2 * r2 + d * d) / (2.0 * d)

    # Squared half-length of the chord.
    h2 = r1 * r1 - a * a

    # Avoid tiny negative values from floating-point errors.
    if h2 < 0:
        h2 = 0.0

    h = math.sqrt(h2)

    # Sign of direction from circle i to circle j.
    sgn = 1.0 if xs[j] > xs[i] else -1.0

    # x-coordinate of chord midpoint.
    mx = xs[i] + a * sgn

    # Two symmetric intersections.
    return [Point(mx, h), Point(mx, -h)]


def equilibrium(S, xs, ls):
    # Find the lowest point in the intersection of active disks.
    if not S:
        return Point(0.0, -1e18)

    candidates = []

    # Bottom point of every active circle.
    for i in S:
        candidates.append(Point(xs[i], -ls[i]))

    # Intersection points of every pair of active circles.
    for a in range(len(S)):
        for b in range(a + 1, len(S)):
            candidates.extend(circle_pair_inters(S[a], S[b], xs, ls))

    # Choose the valid candidate with minimum y.
    best = Point(0.0, 1e18)

    for c in candidates:
        if inside_disks(c, S, xs, ls) and c.y < best.y:
            best = c

    return best


def tight_set(p, S, xs, ls):
    # Return active ropes that are tight at point p.
    tight = []

    for i in S:
        dx = p.x - xs[i]
        dy = p.y
        d = math.sqrt(dx * dx + dy * dy)

        if abs(d - ls[i]) < 1e-6:
            tight.append(i)

    return tight


def descent_dir(p, tight, xs, ls):
    # Gravity direction.
    g = Point(0.0, -1.0)

    # No tight constraints means straight fall.
    if not tight:
        return DirResult(g, -1)

    normals = []

    # Compute outward normals of tight circles.
    for i in tight:
        r = Point(p.x - xs[i], p.y)
        rn = r.norm()

        if rn < 1e-14:
            normals.append(Point(0.0, -1.0))
        else:
            normals.append(r / rn)

    # Test whether gravity itself is feasible.
    g_ok = True
    for nk in normals:
        if g.dot(nk) > 1e-9:
            g_ok = False
            break

    if g_ok:
        tangent_circle = -1

        # If gravity is tangent to some tight circle, mark it as arc circle.
        for k, nk in enumerate(normals):
            if abs(g.dot(nk)) < 1e-9:
                tangent_circle = tight[k]
                break

        return DirResult(g, tangent_circle)

    # Otherwise try projections of gravity onto tangents of tight circles.
    best_dir = Point(0.0, 1.0)
    best_circle = -1
    found = False

    for k, nk in enumerate(normals):
        gn = g.dot(nk)

        # Projection g - (g dot n) n.
        d = Point(-gn * nk.x, -1.0 - gn * nk.y)

        dn = d.norm()

        if dn < 1e-14:
            continue

        d = d / dn

        # Must move downward.
        if d.y >= -1e-12:
            continue

        ok = True

        # Must be feasible for every other tight disk.
        for kk, nkk in enumerate(normals):
            if kk == k:
                continue

            if d.dot(nkk) > 1e-9:
                ok = False
                break

        if not ok:
            continue

        # Prefer the steepest downward direction.
        if not found or d.y < best_dir.y:
            best_dir = d
            best_circle = tight[k]
            found = True

    if not found:
        # No descent direction: already stable.
        return DirResult(Point(0.0, 0.0), -1)

    return DirResult(best_dir, best_circle)


def fwd_angle(frm, to, s):
    # Angular distance from frm to to in direction s = +1 or -1.
    two_pi = 2.0 * PI
    dt = s * (to - frm)
    dt = math.fmod(dt, two_pi)

    if dt < 0:
        dt += two_pi

    return dt


def trace_path(start, target, S, xs, ls):
    # Build vertical/arc segments from start to target.
    segs = []
    cur = start
    two_pi = 2.0 * PI

    # Large cap for safety.
    for _ in range(20000):
        if (cur - target).norm() < 1e-9:
            break

        tight = tight_set(cur, S, xs, ls)
        dr = descent_dir(cur, tight, xs, ls)

        if dr.dir.norm() < 1e-9:
            break

        if dr.arc_circle == -1:
            # Vertical free fall.
            y_end = -1e18

            for i in S:
                # Skip already tight ropes.
                if i in tight:
                    continue

                dx = cur.x - xs[i]
                disc = ls[i] * ls[i] - dx * dx

                if disc < 0:
                    continue

                y_b = -math.sqrt(disc)

                # First boundary encountered below current point.
                if y_b < cur.y - 1e-12 and y_b > y_end:
                    y_end = y_b

            # If no boundary is hit, go to target y.
            if y_end < -1e17:
                y_end = target.y

            # Clamp if target is on the same vertical line.
            if abs(cur.x - target.x) < 1e-9 and y_end < target.y - 1e-12:
                y_end = target.y

            length = cur.y - y_end

            if length < 1e-12:
                break

            segs.append(Segment(
                is_vert=True,
                x=cur.x,
                y0=cur.y,
                y1=y_end,
                length=length
            ))

            cur = Point(cur.x, y_end)

        else:
            # Move along an arc of circle arc_i.
            arc_i = dr.arc_circle

            theta0 = math.atan2(cur.y, cur.x - xs[arc_i])

            # Positive angular tangent.
            tang = Point(-math.sin(theta0), math.cos(theta0))

            # Choose angular direction matching descent direction.
            s = 1.0 if tang.dot(dr.dir) > 0 else -1.0

            best_dt = two_pi
            best_pt = cur
            have_event = False

            # Events: intersections with other active circles.
            for j in S:
                if j == arc_i:
                    continue

                for q in circle_pair_inters(arc_i, j, xs, ls):
                    tq = math.atan2(q.y, q.x - xs[arc_i])
                    dt = fwd_angle(theta0, tq, s)

                    if dt < 1e-9:
                        continue

                    if dt < best_dt:
                        best_dt = dt
                        best_pt = q
                        have_event = True

            # Event: bottom of this circle.
            tq = -PI / 2.0
            dt = fwd_angle(theta0, tq, s)

            if dt > 1e-9 and dt < best_dt:
                best_dt = dt
                best_pt = Point(xs[arc_i], -ls[arc_i])
                have_event = True

            if not have_event:
                break

            theta1 = theta0 + s * best_dt
            length = ls[arc_i] * best_dt

            if length < 1e-12:
                break

            segs.append(Segment(
                is_vert=False,
                cx=xs[arc_i],
                r=ls[arc_i],
                theta0=theta0,
                theta1=theta1,
                length=length
            ))

            cur = best_pt

        if (cur - target).norm() < 1e-7:
            cur = target
            break

    return segs


def interpolate(phase, s):
    # Return point after traveling distance s in this phase.
    if s <= 0:
        return phase.start_p

    if s >= phase.total_len - 1e-12:
        return phase.end_p

    acc = 0.0

    for sg in phase.segs:
        if s <= acc + sg.length + 1e-12:
            local = s - acc
            frac = local / sg.length if sg.length > 1e-14 else 0.0

            if sg.is_vert:
                # Linear interpolation on vertical segment.
                return Point(
                    sg.x,
                    sg.y0 + (sg.y1 - sg.y0) * frac
                )
            else:
                # Angular interpolation on circular arc.
                theta = sg.theta0 + (sg.theta1 - sg.theta0) * frac
                return Point(
                    sg.cx + sg.r * math.cos(theta),
                    sg.r * math.sin(theta)
                )

        acc += sg.length

    return phase.end_p


def solve():
    data = sys.stdin.read().strip().split()

    if not data:
        return

    ptr = 0

    n = int(data[ptr])
    ptr += 1

    m = int(data[ptr])
    ptr += 1

    xs = [float(data[ptr + i]) for i in range(n)]
    ptr += n

    ls = [float(data[ptr + i]) for i in range(n)]
    ptr += n

    # Convert cut order to zero-based indices.
    cut_seq = [int(data[ptr + i]) - 1 for i in range(n - 1)]
    ptr += n - 1

    ts = [float(data[ptr + i]) for i in range(m)]

    # Initially all ropes are active.
    active = list(range(n))

    # Initial equilibrium.
    cur = equilibrium(active, xs, ls)

    cur_time = 0.0
    phases = []
    final_pos = cur

    # Precompute all movement phases.
    for cut in cut_seq:
        # Remove cut rope.
        active.remove(cut)

        # New equilibrium after cutting.
        new_eq = equilibrium(active, xs, ls)

        # If unchanged, next cut is immediate.
        if (new_eq - cur).norm() < 1e-9:
            cur = new_eq
            final_pos = cur
            continue

        # Build trajectory.
        segs = trace_path(cur, new_eq, active, xs, ls)

        # Total duration equals path length.
        total = sum(sg.length for sg in segs)

        if total < 1e-9:
            cur = new_eq
            final_pos = cur
            continue

        phases.append(Phase(
            start_t=cur_time,
            total_len=total,
            start_p=cur,
            end_p=new_eq,
            segs=segs
        ))

        cur = new_eq
        cur_time += total
        final_pos = cur

    out = []

    # Answer queries.
    for t in ts:
        # If no phase contains t, ball is already at final position.
        pos = final_pos

        for ph in phases:
            if t <= ph.start_t + ph.total_len + 1e-9:
                s = t - ph.start_t

                if s < 0:
                    s = 0.0

                if s > ph.total_len:
                    s = ph.total_len

                pos = interpolate(ph, s)
                break

        out.append(f"{pos.x:.10f} {pos.y:.10f}")

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
