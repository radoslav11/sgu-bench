import sys
import math

EPS = 1e-9

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    # vector ops
    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, k):     return Point(self.x * k, self.y * k)  # scalar
    def __truediv__(self, k): return Point(self.x / k, self.y / k)

    # dot and cross
    def dot(self, other):   return self.x * other.x + self.y * other.y
    def cross(self, other): return self.x * other.y - self.y * other.x

    def norm2(self): return self.x * self.x + self.y * self.y
    def norm(self):  return math.sqrt(self.norm2())

    def perp(self):
        # rotate by +90 degrees
        return Point(-self.y, self.x)


def solve_case(x0, y0, R, xa, ya, xb, yb):
    # Shift coordinates so circle center becomes origin
    O = Point(x0, y0)
    A = Point(xa, ya) - O
    B = Point(xb, yb) - O
    R = float(R)

    # Midpoint of AB
    M = (A + B) / 2.0

    # Perpendicular direction to AB (not normalized)
    V = (B - A).perp()

    # D = |AB|
    D = (B - A).norm()
    Dsq = D * D

    # Quadratic coefficients as in the C++ solution
    b = 2.0 * M.dot(V)
    q = M.norm2() - R * R - Dsq / 4.0

    qa = b * b - 4.0 * R * R * Dsq
    qb = 2.0 * b * q
    qc = q * q - R * R * Dsq

    ts = []
    if abs(qa) < 1e-18:
        # linear qb*t + qc = 0
        if abs(qb) > 1e-18:
            ts.append(-qc / qb)
    else:
        disc = qb * qb - 4.0 * qa * qc
        if disc < 0.0:
            disc = 0.0
        sq = math.sqrt(disc)
        ts.append((-qb + sq) / (2.0 * qa))
        ts.append((-qb - sq) / (2.0 * qa))

    best_cos = 2.0
    best_C = None

    for t in ts:
        # Center P of the circle through A and B
        P = M + (V * t)

        # Radius of that circle
        r = D * math.sqrt(t * t + 0.25)

        pnorm = P.norm()
        if pnorm < EPS:
            continue

        # Try both points on the given circle along the line OP
        for sign in (1.0, -1.0):
            C = P * (sign * R / pnorm)

            # Must also lie on circle centered at P with radius r (tangency)
            if abs((C - P).norm() - r) > 1e-6:
                continue

            ua = A - C
            ub = B - C
            na = ua.norm()
            nb = ub.norm()
            if na < EPS or nb < EPS:
                continue

            cosv = ua.dot(ub) / (na * nb)
            if cosv < best_cos:
                best_cos = cosv
                best_C = C

    # Translate back to original coordinates
    ans = best_C + O
    return ans.x, ans.y


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    idx = 1
    out_lines = []
    for _ in range(t):
        x0 = int(data[idx]); y0 = int(data[idx+1]); R = int(data[idx+2])
        xa = int(data[idx+3]); ya = int(data[idx+4])
        xb = int(data[idx+5]); yb = int(data[idx+6])
        idx += 7

        x, y = solve_case(x0, y0, R, xa, ya, xb, yb)
        out_lines.append(f"{x:.6f} {y:.6f}")

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()
