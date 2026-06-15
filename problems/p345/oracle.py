import sys
import math


EPS = 1e-9


def cross(p, q):
    """
    Returns the 2D cross product p ^ q.
    """
    return p[0] * q[1] - p[1] * q[0]


def first_true(lo, hi, pred):
    """
    Binary search for the first integer x in [lo, hi)
    such that pred(x) is True.

    Assumes pred is monotone:
    False, False, ..., True, True, ...
    """
    while lo < hi:
        mid = (lo + hi) // 2

        if pred(mid):
            hi = mid
        else:
            lo = mid + 1

    return lo


def line_intersection(p1, p2, a, d):
    """
    Intersects the infinite line p1-p2 with the query line a + t*d.

    p1, p2: endpoints of a polygon edge.
    a: one point on the query line.
    d: direction vector of the query line.
    """
    edge = (p2[0] - p1[0], p2[1] - p1[1])

    numerator = cross((a[0] - p1[0], a[1] - p1[1]), d)
    denominator = cross(edge, d)

    t = numerator / denominator

    return (
        p1[0] + edge[0] * t,
        p1[1] + edge[1] * t,
    )


def solve():
    data = sys.stdin.buffer.read().split()
    ptr = 0

    n = int(data[ptr])
    ptr += 1

    pts = []

    for _ in range(n):
        x = float(data[ptr])
        y = float(data[ptr + 1])
        ptr += 2
        pts.append((x, y))

    # Compute doubled signed polygon area.
    area2 = 0.0

    for i in range(n):
        area2 += cross(pts[i], pts[(i + 1) % n])

    # Make polygon counterclockwise.
    if area2 < 0:
        pts.reverse()
        area2 = -area2

    total_area = area2 / 2.0

    # Prefix sums of shoelace terms.
    pref = [0.0] * (n + 1)

    for i in range(n):
        pref[i + 1] = pref[i] + cross(pts[i], pts[(i + 1) % n])

    # Find lexicographically leftmost and rightmost vertices.
    l_idx = 0
    r_idx = 0

    for i in range(1, n):
        if pts[i] < pts[l_idx]:
            l_idx = i

        if pts[r_idx] < pts[i]:
            r_idx = i

    # Chain from leftmost to rightmost.
    lower_len = (r_idx - l_idx) % n + 1

    # Chain from rightmost to leftmost.
    upper_len = (l_idx - r_idx) % n + 1

    # Duplicate points for easy cyclic indexing.
    pts2 = pts + pts

    q = int(data[ptr])
    ptr += 1

    out = []

    for _ in range(q):
        ax = float(data[ptr])
        ay = float(data[ptr + 1])
        bx = float(data[ptr + 2])
        by = float(data[ptr + 3])
        ptr += 4

        a = (ax, ay)
        d = (bx - ax, by - ay)

        def side(i):
            """
            Signed side of pts2[i] relative to the directed query line.
            """
            p = pts2[i]
            return cross(d, (p[0] - ax, p[1] - ay))

        def extreme_on_chain(start, length, want_max):
            """
            Finds the offset of a maximum or minimum side value
            on a convex chain.

            Because the side values along a convex chain are unimodal,
            binary search on neighboring values is enough.
            """
            lo = 0
            hi = length - 1

            while lo < hi:
                mid = (lo + hi) // 2

                sm = side(start + mid)
                sn = side(start + mid + 1)

                if want_max:
                    if sm < sn:
                        lo = mid + 1
                    else:
                        hi = mid
                else:
                    if sm > sn:
                        lo = mid + 1
                    else:
                        hi = mid

            best = lo
            best_val = side(start + best)

            # Check left endpoint.
            v0 = side(start)

            if want_max:
                if v0 > best_val:
                    best = 0
                    best_val = v0
            else:
                if v0 < best_val:
                    best = 0
                    best_val = v0

            # Check right endpoint.
            v_last = side(start + length - 1)

            if want_max:
                if v_last > best_val:
                    best = length - 1
            else:
                if v_last < best_val:
                    best = length - 1

            return best

        # Global maximum side vertex.
        max_lower_k = extreme_on_chain(l_idx, lower_len, True)
        max_upper_k = extreme_on_chain(r_idx, upper_len, True)

        max_lower_v = side(l_idx + max_lower_k)
        max_upper_v = side(r_idx + max_upper_k)

        if max_lower_v >= max_upper_v:
            i_max = (l_idx + max_lower_k) % n
        else:
            i_max = (r_idx + max_upper_k) % n

        # No positive vertex means no proper split.
        if side(i_max) <= EPS:
            out.append("0.000000")
            continue

        # Global minimum side vertex.
        min_lower_k = extreme_on_chain(l_idx, lower_len, False)
        min_upper_k = extreme_on_chain(r_idx, upper_len, False)

        min_lower_v = side(l_idx + min_lower_k)
        min_upper_v = side(r_idx + min_upper_k)

        if min_lower_v <= min_upper_v:
            i_min = (l_idx + min_lower_k) % n
        else:
            i_min = (r_idx + min_upper_k) % n

        # No negative vertex means no proper split.
        if side(i_min) >= -EPS:
            out.append("0.000000")
            continue

        # Counterclockwise distance from i_min to i_max.
        len_up = (i_max - i_min) % n

        # Counterclockwise distance from i_max to i_min.
        len_dn = (i_min - i_max) % n

        # First positive vertex from i_min to i_max.
        ccw_first_k = first_true(
            1,
            len_up + 1,
            lambda k: side(i_min + k) > 0
        )

        # Last positive vertex from i_max toward i_min.
        ccw_last_k = first_true(
            1,
            len_dn,
            lambda k: side(i_max + k) <= 0
        ) - 1

        def wrap(i):
            """
            Converts an index in the duplicated array back to [0, n).
            """
            return i if i < n else i - n

        ccw_first = wrap(i_min + ccw_first_k)
        ccw_last = wrap(i_max + ccw_last_k)

        prev_f = n - 1 if ccw_first == 0 else ccw_first - 1
        next_l = 0 if ccw_last + 1 == n else ccw_last + 1

        # Boundary intersections with the cutting line.
        x = line_intersection(pts[prev_f], pts[ccw_first], a, d)
        y = line_intersection(pts[ccw_last], pts[next_l], a, d)

        # Shoelace contribution of polygon vertices ccw_first ... ccw_last.
        if ccw_first <= ccw_last:
            chain = pref[ccw_last] - pref[ccw_first]
        else:
            chain = pref[n] - pref[ccw_first] + pref[ccw_last]

        # Doubled area of the positive-side piece.
        doubled = (
            cross(x, pts[ccw_first])
            + chain
            + cross(pts[ccw_last], y)
            + cross(y, x)
        )

        piece = abs(doubled) / 2.0

        ans = min(piece, total_area - piece)

        # Clamp tiny negative floating point errors.
        if ans < 0:
            ans = 0.0

        out.append(f"{ans:.6f}")

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
