import sys
import math


# Small epsilon for floating-point comparisons.
EPS = 1e-9

# Pi constant.
PI = math.pi

# Large number used as infinity.
INF = 10**100


class Point:
    """Simple 2D point/vector class."""

    def __init__(self, x=0.0, y=0.0):
        # Store coordinates as floats.
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other):
        # Vector addition.
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        # Vector subtraction.
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, value):
        # If multiplying by another point, return dot product.
        if isinstance(value, Point):
            return self.x * value.x + self.y * value.y

        # Otherwise multiply by scalar.
        return Point(self.x * value, self.y * value)

    def __rmul__(self, value):
        # Scalar multiplication from the left.
        return self.__mul__(value)

    def __truediv__(self, value):
        # Divide vector by scalar.
        return Point(self.x / value, self.y / value)

    def cross(self, other):
        # 2D cross product.
        return self.x * other.y - self.y * other.x

    def norm2(self):
        # Squared Euclidean length.
        return self.x * self.x + self.y * self.y

    def norm(self):
        # Euclidean length.
        return math.sqrt(self.norm2())

    def angle(self):
        # Polar angle of the vector.
        return math.atan2(self.y, self.x)


def dist(a, b):
    # Distance between two points.
    return (a - b).norm()


def main():
    # Read all numbers from standard input.
    data = list(map(float, sys.stdin.read().split()))

    # First line: coordinates of A and B.
    xa, ya, xb, yb = data[0:4]

    # Second line: center and radius.
    xc, yc, R = data[4:7]

    # Third line: fuel distance.
    D = data[7]

    # Build point objects.
    A = Point(xa, ya)
    B = Point(xb, yb)
    center = Point(xc, yc)

    def on_circle(angle):
        """
        Return the point on the crater circle corresponding to polar angle
        measured from the center.
        """
        return center + Point(math.cos(angle), math.sin(angle)) * R

    def seg_ok(p0, p1):
        """
        Return True if segment p0-p1 does not enter the disk.
        Touching the boundary is allowed.
        """
        # Segment vector.
        d = p1 - p0

        # Squared segment length.
        dn = d.norm2()

        # Parameter of closest point to center.
        t = 0.0

        if dn > EPS:
            # Project center onto the supporting line.
            t = ((center - p0) * d) / dn

            # Clamp projection to the finite segment.
            t = max(0.0, min(1.0, t))

        # Closest point on segment to center.
        closest = p0 + d * t

        # Segment is outside if closest distance is at least R.
        return (closest - center).norm() >= R - 1e-7

    def print_route(parts):
        """
        Print route parts, removing zero-length pieces.
        Each part is stored as (type, point1, point2).
        """
        filtered = []

        # Remove degenerate parts.
        for typ, p, q in parts:
            if dist(p, q) > 1e-9:
                filtered.append((typ, p, q))

        # Print number of parts.
        print(len(filtered))

        # Print each part.
        for typ, p, q in filtered:
            print(
                f"{typ} "
                f"{p.x:.8f} {p.y:.8f} "
                f"{q.x:.8f} {q.y:.8f}"
            )

    # If direct segment does not enter crater, it is the shortest route.
    if seg_ok(A, B):
        print_route([("S", A, B)])
        return

    # Distance from center to line AB.
    h = abs((B - A).cross(center - A)) / dist(A, B)

    # Length of the chord of the circle cut by line AB.
    chord_ab = 2.0 * math.sqrt(max(0.0, R * R - h * h))

    # If fuel is enough for the direct crossing, direct segment is optimal.
    if chord_ab <= D + 1e-9:
        print_route([("S", A, B)])
        return

    # Distances from center to A and B.
    dA = dist(A, center)
    dB = dist(B, center)

    # Lengths of tangent segments.
    tA = math.sqrt(dA * dA - R * R)
    tB = math.sqrt(dB * dB - R * R)

    # Polar angles of A and B with respect to center.
    psiA = (A - center).angle()
    psiB = (B - center).angle()

    # Angular offsets to tangent points.
    gA = math.acos(R / dA)
    gB = math.acos(R / dB)

    # Central angle subtended by chord of length D.
    # Here D < direct chord length <= 2R, because otherwise we returned above.
    theta_d = 2.0 * math.asin(min(1.0, D / (2.0 * R)))

    # Best answer found so far.
    best_len = INF
    best_parts = []

    def consider(length, parts):
        """Update global best route if this candidate is shorter."""
        nonlocal best_len, best_parts

        if length < best_len:
            best_len = length
            best_parts = parts

    # Try both clockwise and counterclockwise sides.
    for s in (1, -1):
        # Tangent point angle from A on this side.
        a1 = psiA + s * gA

        # Tangent point angle to B on this side.
        a2 = psiB - s * gB

        # Angular size of boundary arc from a1 to a2 in direction s.
        Phi = math.fmod(s * (a2 - a1), 2.0 * PI)

        # Normalize to non-negative.
        if Phi < 0.0:
            Phi += 2.0 * PI

        # Actual tangent points.
        Ta = on_circle(a1)
        Tb = on_circle(a2)

        # Candidate 1:
        # A -> Ta, then boundary arc Ta -> Tb, then Tb -> B.
        consider(
            tA + R * Phi + tB,
            [("S", A, Ta), ("A", Ta, Tb), ("S", Tb, B)]
        )

        # Candidate 2:
        # Replace a part of this arc by a chord of length D.
        if theta_d <= Phi + 1e-12:
            # Endpoint after crossing the chord.
            P2 = on_circle(a1 + s * theta_d)

            # Length = tangents + chord + remaining arc.
            consider(
                tA + D + R * (Phi - theta_d) + tB,
                [("S", A, Ta), ("S", Ta, P2), ("A", P2, Tb), ("S", Tb, B)]
            )

        def free_chord_cost(alpha):
            """
            Cost of route A -> P -> Q -> B,
            where angle(P)=alpha and angle(Q)=alpha+s*theta_d.
            """
            P = on_circle(alpha)
            Q = on_circle(alpha + s * theta_d)

            # Outer segments must be outside the disk.
            if not seg_ok(A, P):
                return INF

            if not seg_ok(Q, B):
                return INF

            # Total length.
            return dist(A, P) + D + dist(Q, B)

        # For Q to be visible from B:
        # alpha + s*theta_d must be within B's tangent interval.
        # Therefore alpha is centered near psiB - s*theta_d.
        cB = psiB - s * theta_d

        # Signed shortest angular difference between cB and psiA.
        delta = math.atan2(math.sin(cB - psiA), math.cos(cB - psiA))

        # Intersect A-visible interval [-gA, gA] with shifted B-visible interval.
        lo = max(-gA, delta - gB)
        hi = min(gA, delta + gB)

        # Candidate 3:
        # If feasible, minimize A -> P -> Q -> B over alpha.
        if lo <= hi:
            # Convert relative angles to absolute angles.
            lo += psiA
            hi += psiA

            # Ternary search over the feasible interval.
            for _ in range(200):
                m1 = lo + (hi - lo) / 3.0
                m2 = hi - (hi - lo) / 3.0

                if free_chord_cost(m1) < free_chord_cost(m2):
                    hi = m2
                else:
                    lo = m1

            # Best alpha.
            alpha = (lo + hi) / 2.0

            # Candidate cost.
            cost = free_chord_cost(alpha)

            # If feasible, save route.
            if cost < INF / 2:
                P = on_circle(alpha)
                Q = on_circle(alpha + s * theta_d)

                consider(
                    cost,
                    [("S", A, P), ("S", P, Q), ("S", Q, B)]
                )

    # Output shortest route found.
    print_route(best_parts)


if __name__ == "__main__":
    main()
