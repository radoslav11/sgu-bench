import sys
import math
from itertools import combinations


# ---------- Basic vector operations ----------

def sub(a, b):
    """Return a - b for 3D vectors."""
    return [a[0] - b[0], a[1] - b[1], a[2] - b[2]]


def dot(a, b):
    """Return dot product of two 3D vectors."""
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def cross(a, b):
    """Return cross product of two 3D vectors."""
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ]


# ---------- Input ----------

data = sys.stdin.read().strip().split()

W = float(data[0])
H = float(data[1])
D = float(data[2])

n = int(data[3])

ghosts = []
ptr = 4

for _ in range(n):
    x = float(data[ptr])
    y = float(data[ptr + 1])
    z = float(data[ptr + 2])
    r = float(data[ptr + 3])
    ptr += 4
    ghosts.append((x, y, z, r))


# Global witness point.
witness = [W / 2.0, H / 2.0, D / 2.0]


def valid_center(p, r):
    """
    Check whether p can be the center of a new ghost with radius r.
    """
    eps = 1e-7

    # Check box constraints.
    if p[0] < r - eps or p[0] > W - r + eps:
        return False
    if p[1] < r - eps or p[1] > H - r + eps:
        return False
    if p[2] < r - eps or p[2] > D - r + eps:
        return False

    # Check distance from every old ghost.
    for gx, gy, gz, gr in ghosts:
        dx = p[0] - gx
        dy = p[1] - gy
        dz = p[2] - gz

        need = r + gr
        dist2 = dx * dx + dy * dy + dz * dz

        if dist2 < (need - eps) * (need - eps):
            return False

    return True


def line_sphere(n1, d1, n2, d2, c, rho, r):
    """
    Intersect:
        n1 · p = d1
        n2 · p = d2
        |p - c| = rho

    Test produced points as possible centers.
    """
    global witness

    # Direction of intersection line of the two planes.
    u = cross(n1, n2)
    lu2 = dot(u, u)

    # Parallel or almost-parallel planes.
    if lu2 < 1e-18:
        return False

    # Find one point p0 on the line of intersection.
    t1 = cross(n2, u)
    t2 = cross(u, n1)

    p0 = [
        (d1 * t1[i] + d2 * t2[i]) / lu2
        for i in range(3)
    ]

    # Parametrize line as p(t) = p0 + t*u.
    w = sub(p0, c)

    # Quadratic equation:
    # |p0 + t*u - c|^2 = rho^2
    b = 2.0 * dot(u, w)
    cc = dot(w, w) - rho * rho

    disc = b * b - 4.0 * lu2 * cc

    # If negative only due to floating error, clamp to zero.
    if disc < 0.0:
        disc = 0.0

    sq = math.sqrt(disc)

    # Try both roots.
    for sign in (-1.0, 1.0):
        t = (-b + sign * sq) / (2.0 * lu2)

        p = [
            p0[0] + t * u[0],
            p0[1] + t * u[1],
            p0[2] + t * u[2],
        ]

        if valid_center(p, r):
            witness = p
            return True

    return False


def feasible(r):
    """
    Return True if a new ghost of radius r can be placed.
    Also stores one valid center in witness.
    """
    global witness

    # Radius too large for the box.
    if 2.0 * r > W or 2.0 * r > H or 2.0 * r > D:
        return False

    # There are six box planes and n ghost spheres.
    m = 6 + n

    # Normals of six box planes.
    plane_normals = [
        [1.0, 0.0, 0.0],
        [1.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 0.0, 1.0],
        [0.0, 0.0, 1.0],
    ]

    # Constants of six box planes.
    plane_ds = [
        r,
        W - r,
        r,
        H - r,
        r,
        D - r,
    ]

    def sphere_center(surface_id):
        """Return center of ghost sphere surface."""
        gx, gy, gz, _ = ghosts[surface_id - 6]
        return [gx, gy, gz]

    def sphere_radius(surface_id):
        """Return inflated radius of ghost sphere surface."""
        return ghosts[surface_id - 6][3] + r

    # Enumerate all triples of surfaces.
    for a, b, c in combinations(range(m), 3):
        ids = [a, b, c]

        # Pick one sphere from the triple, if any.
        sph = -1
        for surface_id in ids:
            if surface_id >= 6:
                sph = surface_id

        # Case 1: three box planes.
        if sph == -1:
            # They must correspond to three different axes.
            axes = [surface_id // 2 for surface_id in ids]

            if len(set(axes)) == 3:
                p = [0.0, 0.0, 0.0]

                for surface_id in ids:
                    axis = surface_id // 2
                    p[axis] = plane_ds[surface_id]

                if valid_center(p, r):
                    witness = p
                    return True

            continue

        # Case 2: at least one sphere.
        # Use this sphere as the main sphere.
        rc = sphere_center(sph)
        rrho = sphere_radius(sph)

        # Convert the other two surfaces into planes.
        planes_n = []
        planes_d = []

        for surface_id in ids:
            if surface_id == sph:
                continue

            if surface_id < 6:
                # Box plane.
                planes_n.append(plane_normals[surface_id])
                planes_d.append(plane_ds[surface_id])
            else:
                # Sphere converted to radical plane.
                oc = sphere_center(surface_id)
                orho = sphere_radius(surface_id)

                normal = [
                    2.0 * (oc[0] - rc[0]),
                    2.0 * (oc[1] - rc[1]),
                    2.0 * (oc[2] - rc[2]),
                ]

                dval = (
                    rrho * rrho
                    - orho * orho
                    - dot(rc, rc)
                    + dot(oc, oc)
                )

                planes_n.append(normal)
                planes_d.append(dval)

        # Intersect two planes and the main sphere.
        if line_sphere(
            planes_n[0],
            planes_d[0],
            planes_n[1],
            planes_d[1],
            rc,
            rrho,
            r,
        ):
            return True

    return False


# ---------- Binary search ----------

lo = 0.0
hi = min(W, H, D) / 2.0

# 60 iterations are plenty for millimeter accuracy.
for _ in range(60):
    mid = (lo + hi) / 2.0

    if feasible(mid):
        lo = mid
    else:
        hi = mid

# Recover witness for final radius.
feasible(lo)

print("{:.12f} {:.12f} {:.12f}".format(witness[0], witness[1], witness[2]))
