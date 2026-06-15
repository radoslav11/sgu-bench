import sys
import math

EPS = 1e-6

class Vec3:
    """Simple 3D vector class with basic operations."""
    __slots__ = ("x", "y", "z")

    def __init__(self, x=0.0, y=0.0, z=0.0):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def __add__(self, other):
        """Vector addition: self + other"""
        return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        """Vector subtraction: self - other"""
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, k):
        """Scalar multiplication: self * k"""
        return Vec3(self.x * k, self.y * k, self.z * k)

    # For k * v, Python will call __rmul__
    __rmul__ = __mul__

    def dot(self, other):
        """Dot product with another Vec3."""
        return self.x * other.x + self.y * other.y + self.z * other.z

    def norm(self):
        """Euclidean length of the vector."""
        return math.sqrt(self.dot(self))

    def normalize(self):
        """Return a normalized (unit length) copy of the vector."""
        n = self.norm()
        # Problem guarantees we won't have zero-length directions
        return Vec3(self.x / n, self.y / n, self.z / n)


class Sphere:
    """Sphere with center (Vec3) and radius r."""
    __slots__ = ("center", "r")

    def __init__(self, center, r):
        self.center = center
        self.r = float(r)


def ray_sphere_intersect(O, D, sphere):
    """
    Compute intersection of ray P(t) = O + t*D with sphere.
    Return smallest t > EPS, or -1 if no such intersection.
    """
    # Vector from sphere center to ray origin
    V = O - sphere.center

    # Quadratic coefficients: a*t^2 + b*t + c = 0
    a = D.dot(D)              # usually 1.0 if D is normalized
    b = 2.0 * V.dot(D)
    c = V.dot(V) - sphere.r * sphere.r

    # Discriminant
    disc = b * b - 4.0 * a * c

    # No real roots: discriminant negative beyond tolerance
    if disc < -EPS:
        return -1.0

    # Clamp small negative values to 0 before sqrt for numerical safety
    disc = math.sqrt(max(0.0, disc))

    # Two candidate intersection parameters
    t1 = (-b - disc) / (2.0 * a)
    t2 = (-b + disc) / (2.0 * a)

    # We need smallest positive t (t > EPS)
    if t1 > EPS:
        return t1
    if t2 > EPS:
        return t2

    # Both are behind or very close to origin -> no valid intersection
    return -1.0


def reflect(D, N):
    """
    Reflect direction D around normal N.
    Reflection formula: R = D - 2(D·N)N
    Both D and N are Vec3.
    """
    k = 2.0 * D.dot(N)
    return D - (k * N)


def solve_one_case(data_iter):
    """
    Read a single test case from iterator of tokens and print result.
    The problem as given uses a single test per file.
    """
    # Read number of spheres
    try:
        n = int(next(data_iter))
    except StopIteration:
        return False  # no more data

    spheres = []

    # Read n spheres: xi yi zi ri
    for _ in range(n):
        x = float(next(data_iter))
        y = float(next(data_iter))
        z = float(next(data_iter))
        r = float(next(data_iter))
        spheres.append(Sphere(Vec3(x, y, z), r))

    # Last line: 6 real numbers for two points
    x1 = float(next(data_iter))
    y1 = float(next(data_iter))
    z1 = float(next(data_iter))
    x2 = float(next(data_iter))
    y2 = float(next(data_iter))
    z2 = float(next(data_iter))

    # Ray origin
    origin = Vec3(x1, y1, z1)

    # Direction from first point to second, normalized
    dir_vec = Vec3(x2 - x1, y2 - y1, z2 - z1).normalize()

    hits = []          # list of sphere indices (1-based) hit in order
    last_hit = -1      # index (0-based) of last hit sphere; -1 if none

    # Simulate up to 11 hits (one extra to detect "more than 10")
    for step in range(11):
        best_t = float('inf')  # nearest intersection distance along ray
        best_idx = -1          # index of sphere giving that intersection

        # Check intersection with each sphere
        for i, s in enumerate(spheres):
            # Skip last-hit sphere to avoid immediate re-hitting the same one
            if i == last_hit:
                continue

            t = ray_sphere_intersect(origin, dir_vec, s)

            # We only care about positive t; choose the smallest
            if t > 0.0 and t < best_t:
                best_t = t
                best_idx = i

        # If we didn't find any intersection, the ray escapes
        if best_idx == -1:
            break

        # Record 1-based index of this sphere
        hits.append(best_idx + 1)

        # Update last hit
        last_hit = best_idx

        # Compute hit point P = origin + dir_vec * best_t
        P = origin + dir_vec * best_t

        # Compute outward normal at the hit point and normalize
        N = (P - spheres[best_idx].center).normalize()

        # Compute reflected direction and normalize to unit length
        dir_vec = reflect(dir_vec, N).normalize()

        # New ray origin is the hit point
        origin = P

    # Output logic: print first up to 10 hits, separated by spaces
    out_hits = hits[:10]
    if out_hits:
        sys.stdout.write(" ".join(str(x) for x in out_hits))
    # If more than 10 hits, append " etc."
    if len(hits) > 10:
        if out_hits:
            sys.stdout.write(" ")
        sys.stdout.write("etc.")
    sys.stdout.write("\n")

    return True


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    while True:
        if not solve_one_case(it):
            break


if __name__ == "__main__":
    main()
