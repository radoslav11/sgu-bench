import sys
import math

# --------- Basic 2D point utilities ---------

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, k: float):
        # scalar multiplication
        return Point(self.x * k, self.y * k)

    def __truediv__(self, k: float):
        return Point(self.x / k, self.y / k)

    def norm(self) -> float:
        # Euclidean length
        return math.hypot(self.x, self.y)

    def perp(self):
        # rotate +90 degrees: (x,y) -> (-y,x)
        return Point(-self.y, self.x)

    def unit(self):
        # unit vector in same direction
        length = self.norm()
        return self / length

    def rotate(self, a: float):
        # rotate around origin by angle a (radians, CCW positive)
        ca = math.cos(a)
        sa = math.sin(a)
        return Point(self.x * ca - self.y * sa, self.x * sa + self.y * ca)


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return

    it = iter(data)

    # Read N, N1, N2
    n = int(next(it))
    n1 = int(next(it))
    n2 = int(next(it))

    # Read coordinates of vertex N1 and N2
    A = Point(float(next(it)), float(next(it)))
    B = Point(float(next(it)), float(next(it)))

    # k = number of clockwise steps from n1 to n2
    k = (n2 - n1 + n) % n

    # Central angle between A and B
    theta = 2.0 * math.pi * k / n

    # Midpoint of chord AB
    mid = (A + B) / 2.0

    # Half chord length
    half_len = (B - A).norm() / 2.0

    # Circumradius from chord-length formula
    R = half_len / math.sin(theta / 2.0)

    # Unit perpendicular direction to chord (choose a consistent side)
    perp = (A - B).perp().unit()

    # Center is offset from midpoint by R*cos(theta/2)
    O = mid + perp * (R * math.cos(theta / 2.0))

    # Output all vertices in order 1..N
    out_lines = []
    for i in range(1, n + 1):
        # Clockwise step corresponds to negative rotation angle
        angle = -2.0 * math.pi * (i - n1) / n
        pt = O + (A - O).rotate(angle)
        out_lines.append(f"{pt.x:.6f} {pt.y:.6f}")

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()
