import math
import sys


# Small value used to handle floating-point comparison errors.
EPS = 1e-9


class Point:
    def __init__(self, x=0.0, y=0.0):
        # Store x-coordinate as float.
        self.x = float(x)

        # Store y-coordinate as float.
        self.y = float(y)

    def __add__(self, other):
        # Vector addition.
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        # Vector subtraction.
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, value):
        # If multiplying by another Point, return dot product.
        if isinstance(value, Point):
            return self.x * value.x + self.y * value.y

        # Otherwise multiply vector by scalar.
        return Point(self.x * value, self.y * value)

    def __rmul__(self, value):
        # Support scalar * Point.
        return self.__mul__(value)

    def __truediv__(self, value):
        # Divide vector by scalar.
        return Point(self.x / value, self.y / value)

    def norm2(self):
        # Squared vector length.
        return self.x * self.x + self.y * self.y

    def norm(self):
        # Vector length.
        return math.sqrt(self.norm2())

    def unit(self):
        # Unit vector in the same direction.
        length = self.norm()
        return self / length

    def __repr__(self):
        # String representation for debugging.
        return f"{self.x} {self.y}"


def main():
    # Read all input tokens.
    data = sys.stdin.read().strip().split()

    # If input is empty, do nothing.
    if not data:
        return

    # Convert all tokens to float.
    data = list(map(float, data))

    # Input format:
    # A: x y vx vy radius mass
    # B: x y vx vy radius mass
    # t
    idx = 0

    # Read disk A position.
    pos_a = Point(data[idx], data[idx + 1])
    idx += 2

    # Read disk A velocity.
    vel_a = Point(data[idx], data[idx + 1])
    idx += 2

    # Read disk A radius.
    r_a = data[idx]
    idx += 1

    # Read disk A mass.
    m_a = data[idx]
    idx += 1

    # Read disk B position.
    pos_b = Point(data[idx], data[idx + 1])
    idx += 2

    # Read disk B velocity.
    vel_b = Point(data[idx], data[idx + 1])
    idx += 2

    # Read disk B radius.
    r_b = data[idx]
    idx += 1

    # Read disk B mass.
    m_b = data[idx]
    idx += 1

    # Read total elapsed time.
    total_time = data[idx]

    # Collision happens when distance between centers equals sum of radii.
    R = r_a + r_b

    # Relative position of A with respect to B.
    p = pos_a - pos_b

    # Relative velocity of A with respect to B.
    v = vel_a - vel_b

    # Quadratic coefficient a in |p + v*s|^2 = R^2.
    a = v * v

    # Quadratic coefficient b.
    b = 2.0 * (p * v)

    # Quadratic coefficient c.
    c = (p * p) - R * R

    # Collision time. None means no collision before total_time.
    hit = None

    # If relative velocity is zero, the distance never changes.
    if a > EPS:
        # Discriminant of the quadratic equation.
        disc = b * b - 4.0 * a * c

        # If discriminant is nonnegative, the relative path reaches the circle.
        if disc >= -EPS:
            # Avoid tiny negative values caused by floating-point errors.
            if disc < 0.0:
                disc = 0.0

            # The earlier root is the first contact time.
            s = (-b - math.sqrt(disc)) / (2.0 * a)

            # Collision is relevant only if it occurs during the interval.
            if s > EPS and s <= total_time + EPS:
                hit = min(s, total_time)

    # Case 1: no collision happens before the target time.
    if hit is None:
        # Move A uniformly.
        pos_a = pos_a + vel_a * total_time

        # Move B uniformly.
        pos_b = pos_b + vel_b * total_time

    # Case 2: collision happens.
    else:
        # Move A to the collision instant.
        pos_a = pos_a + vel_a * hit

        # Move B to the collision instant.
        pos_b = pos_b + vel_b * hit

        # Unit normal vector from B to A at collision.
        n = (pos_a - pos_b).unit()

        # Relative velocity projected onto the collision normal.
        rel = (vel_a - vel_b) * n

        # Elastic collision formula for A.
        vel_a = vel_a - n * (2.0 * m_b / (m_a + m_b) * rel)

        # Elastic collision formula for B.
        vel_b = vel_b + n * (2.0 * m_a / (m_a + m_b) * rel)

        # Remaining time after collision.
        rest = total_time - hit

        # Move A with its new velocity.
        pos_a = pos_a + vel_a * rest

        # Move B with its new velocity.
        pos_b = pos_b + vel_b * rest

    # Print final position and velocity of A.
    print(f"{pos_a.x:.6f} {pos_a.y:.6f} {vel_a.x:.6f} {vel_a.y:.6f}")

    # Print final position and velocity of B.
    print(f"{pos_b.x:.6f} {pos_b.y:.6f} {vel_b.x:.6f} {vel_b.y:.6f}")


if __name__ == "__main__":
    main()
