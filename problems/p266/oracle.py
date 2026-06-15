import math
import sys


def clamp(x, lo=-1.0, hi=1.0):
    """
    Clamp x into the interval [lo, hi].

    This is important before calling acos(), because floating-point
    computations can produce values such as 1.0000000000000002.
    """
    return max(lo, min(hi, x))


def dot(a, b):
    """
    Dot product of two 3D vectors.
    """
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def norm(a):
    """
    Euclidean length of a 3D vector.
    """
    return math.sqrt(dot(a, a))


def main():
    # Read all input numbers.
    data = sys.stdin.read().strip().split()

    # If input is empty, do nothing.
    if not data:
        return

    # First value is the radius of the planet.
    R = float(data[0])

    # Next three values are coordinates of scanner 1.
    p1 = (
        float(data[1]),
        float(data[2]),
        float(data[3]),
    )

    # Last three values are coordinates of scanner 2.
    p2 = (
        float(data[4]),
        float(data[5]),
        float(data[6]),
    )

    # Mathematical constant pi.
    pi = math.pi

    # Distances from the center of the planet to each scanner.
    d1 = norm(p1)
    d2 = norm(p2)

    # For a scanner at distance d, the visible spherical cap has
    # angular radius alpha satisfying cos(alpha) = R / d.
    cos_a = R / d1
    cos_b = R / d2

    # Compute the corresponding sines safely.
    sin_a = math.sqrt(max(0.0, 1.0 - cos_a * cos_a))
    sin_b = math.sqrt(max(0.0, 1.0 - cos_b * cos_b))

    # Angular radii of the two visible caps.
    alpha = math.acos(clamp(cos_a))
    beta = math.acos(clamp(cos_b))

    # Angle theta between scanner direction vectors.
    cos_t = dot(p1, p2) / (d1 * d2)
    cos_t = clamp(cos_t)

    # sin(theta), computed safely.
    sin_t = math.sqrt(max(0.0, 1.0 - cos_t * cos_t))

    # Actual angle theta.
    theta = math.acos(cos_t)

    # Area of each spherical cap:
    # area = 2πR²(1 - cos(radius_angle)).
    area1 = 2.0 * pi * R * R * (1.0 - cos_a)
    area2 = 2.0 * pi * R * R * (1.0 - cos_b)

    # Compute intersection area of the two caps.
    if theta >= alpha + beta:
        # Caps are disjoint or tangent.
        intersection = 0.0

    elif theta + min(alpha, beta) <= max(alpha, beta):
        # One cap lies completely inside the other.
        # Intersection equals the smaller cap.
        if alpha < beta:
            intersection = area1
        else:
            intersection = area2

    else:
        # Partial overlap: the intersection is a spherical lens.

        # Half-angle span of the arc from cap 1's boundary.
        psi_a = math.acos(
            clamp((cos_b - cos_a * cos_t) / (sin_a * sin_t))
        )

        # Half-angle span of the arc from cap 2's boundary.
        psi_b = math.acos(
            clamp((cos_a - cos_b * cos_t) / (sin_b * sin_t))
        )

        # Angle between boundary tangent directions at an intersection point.
        gamma = math.acos(
            clamp((cos_t - cos_a * cos_b) / (sin_a * sin_b))
        )

        # Lens area from Gauss-Bonnet on the unit sphere,
        # then scaled by R².
        intersection = R * R * (
            2.0 * (pi - gamma)
            - 2.0 * psi_a * cos_a
            - 2.0 * psi_b * cos_b
        )

    # Union area by inclusion-exclusion.
    answer = area1 + area2 - intersection

    # Print rounded to 3 digits after the decimal point.
    print(f"{answer:.3f}")


if __name__ == "__main__":
    main()
