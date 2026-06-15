import sys


def sub(a, b):
    """
    Returns vector a - b.
    Points/vectors are represented as tuples (x, y, z).
    """
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2])


def cross(a, b):
    """
    Returns cross product a x b.
    """
    return (
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    )


def dot(a, b):
    """
    Returns dot product a . b.
    """
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def signed_volume(a, b, c, d):
    """
    Returns six times the signed volume of tetrahedron abcd.

    It is positive if d lies on one side of plane abc,
    negative on the other side,
    and zero if a, b, c, d are coplanar.
    """
    return dot(cross(sub(b, a), sub(c, a)), sub(d, a))


def is_zero_vector(v):
    """
    Checks whether a 3D vector is exactly zero.
    """
    return v[0] == 0 and v[1] == 0 and v[2] == 0


def lower_dimensional(points):
    """
    Returns True if all points lie in a space of dimension less than 3:
    a point, a line, or a plane.

    In such cases every input star is considered boundary.
    """
    n = len(points)

    # Fewer than four points cannot form a 3D hull.
    if n < 4:
        return True

    p0 = points[0]

    # Find a point different from p0.
    p1 = None
    for p in points[1:]:
        if p != p0:
            p1 = p
            break

    # All points are identical.
    if p1 is None:
        return True

    # Find a point not collinear with p0 and p1.
    p2 = None
    base = sub(p1, p0)
    for p in points:
        if not is_zero_vector(cross(base, sub(p, p0))):
            p2 = p
            break

    # All points are collinear.
    if p2 is None:
        return True

    # Find a point not coplanar with p0, p1, p2.
    for p in points:
        if signed_volume(p0, p1, p2, p) != 0:
            return False

    # All points are coplanar.
    return True


def main():
    # Read all input lines.
    lines = sys.stdin.read().splitlines()

    # Empty input guard.
    if not lines:
        return

    # First line contains n.
    n = int(lines[0].strip())

    names = []
    points = []

    # Parse each star.
    for i in range(1, n + 1):
        line = lines[i].rstrip("\r")

        # Star names may contain spaces, so parse from the right.
        # The last three space-separated fields are x, y, z.
        name, xs, ys, zs = line.rsplit(" ", 3)

        names.append(name)
        points.append((int(xs), int(ys), int(zs)))

    # If the hull is not truly 3D, all stars are boundary stars.
    if lower_dimensional(points):
        answer = sorted(names)
        print(len(answer))
        print("\n".join(answer))
        return

    # boundary[i] is True if point i lies on some supporting plane.
    boundary = [False] * n

    # Enumerate every triple of points.
    for i in range(n):
        a = points[i]

        for j in range(i + 1, n):
            b = points[j]

            for k in range(j + 1, n):
                c = points[k]

                # Skip collinear triples because they do not define a plane.
                normal = cross(sub(b, a), sub(c, a))
                if is_zero_vector(normal):
                    continue

                has_pos = False
                has_neg = False
                coplanar_indices = []

                # Check all points against the plane through a, b, c.
                for idx, p in enumerate(points):
                    vol = dot(normal, sub(p, a))

                    if vol > 0:
                        has_pos = True
                    elif vol < 0:
                        has_neg = True
                    else:
                        coplanar_indices.append(idx)

                    # If points exist on both sides, this is not a supporting plane.
                    if has_pos and has_neg:
                        break

                # If all points are on one side or on the plane,
                # this plane supports the convex hull.
                if not (has_pos and has_neg):
                    # Every input point lying on that supporting plane is boundary.
                    for idx in coplanar_indices:
                        boundary[idx] = True

    # Collect names of boundary points.
    answer = sorted(names[i] for i in range(n) if boundary[i])

    # Print result.
    print(len(answer))
    print("\n".join(answer))


if __name__ == "__main__":
    main()
