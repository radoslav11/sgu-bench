import sys


def solve():
    # Read all integers from input at once for speed.
    data = list(map(int, sys.stdin.buffer.read().split()))

    # Pointer into the input array.
    ptr = 0

    # Number of test cases.
    T = data[ptr]
    ptr += 1

    # Answers for all test cases.
    answers = []

    # Constants used for packing sweep events.
    #
    # Event format:
    #   shifted_level | position | type | vertex_id_plus_one
    #
    # type:
    #   0 = close
    #   1 = query
    #   2 = open
    VID_BITS = 20
    TYPE_SHIFT = VID_BITS
    POS_SHIFT = VID_BITS + 2
    LEVEL_SHIFT = POS_SHIFT + 17
    LEVEL_OFFSET = 1 << 17
    VID_MASK = (1 << VID_BITS) - 1

    def pack(level, pos, typ, vid):
        """
        Pack an event into one integer.

        level may be negative, so shift it by LEVEL_OFFSET.
        vid == -1 is used for non-query events.
        """
        return (
            ((level + LEVEL_OFFSET) << LEVEL_SHIFT)
            | (pos << POS_SHIFT)
            | (typ << TYPE_SHIFT)
            | (vid + 1)
        )

    def inside_large(p, n):
        """
        Check whether point p is inside the large triangle:
            x >= 0, y >= 0, x + y <= n
        """
        x, y = p
        return x >= 0 and y >= 0 and x + y <= n

    def dot(a, b, c):
        """
        Dot product of vectors AB and AC.
        """
        x1 = b[0] - a[0]
        y1 = b[1] - a[1]
        x2 = c[0] - a[0]
        y2 = c[1] - a[1]
        return x1 * x2 + y1 * y2

    def dist2(a, b):
        """
        Squared distance between a and b.
        """
        dx = b[0] - a[0]
        dy = b[1] - a[1]
        return dx * dx + dy * dy

    def cross(a, b, c):
        """
        Cross product of AB and AC.
        Absolute value equals twice the triangle area.
        """
        x1 = b[0] - a[0]
        y1 = b[1] - a[1]
        x2 = c[0] - a[0]
        y2 = c[1] - a[1]
        return x1 * y2 - y1 * x2

    def axis_aligned(a, b):
        """
        True if segment AB is horizontal or vertical.
        """
        return a[0] == b[0] or a[1] == b[1]

    def right_angle_index(tri):
        """
        Return index of right-angle vertex if tri is a valid small triangle.

        A valid small triangle must:
        - be right,
        - be isosceles,
        - have horizontal/vertical catheti.

        Return -1 if invalid.
        """
        for i in range(3):
            j = (i + 1) % 3
            k = (i + 2) % 3

            a = tri[i]
            b = tri[j]
            c = tri[k]

            # Perpendicular sides.
            if dot(a, b, c) != 0:
                continue

            # Equal lengths.
            if dist2(a, b) != dist2(a, c):
                continue

            # Catheti must be axis-aligned.
            if not axis_aligned(a, b):
                continue

            if not axis_aligned(a, c):
                continue

            return i

        return -1

    def level_pos(d, x, y):
        """
        Map point (x, y) to (level, position) for direction d.

        d = 0: horizontal, level = y,     pos = x
        d = 1: vertical,   level = x,     pos = y
        d = 2: slope +1,   level = y - x, pos = x
        d = 3: slope -1,   level = x + y, pos = x
        """
        if d == 0:
            return y, x
        if d == 1:
            return x, y
        if d == 2:
            return y - x, x
        return x + y, x

    def edge_direction(dx, dy):
        """
        Return direction class of an already valid triangle edge.

        0: horizontal
        1: vertical
        2: slope +1
        3: slope -1
        """
        if dy == 0:
            return 0
        if dx == 0:
            return 1
        if dx == dy:
            return 2
        return 3

    for _ in range(T):
        # Read n and m.
        n = data[ptr]
        m = data[ptr + 1]
        ptr += 2

        # Store triangles.
        triangles = []

        for _i in range(m):
            x1, y1, x2, y2, x3, y3 = data[ptr:ptr + 6]
            ptr += 6

            triangles.append(((x1, y1), (x2, y2), (x3, y3)))

        # right_idx[i] is right-angle vertex of triangle i.
        right_idx = [-1] * m

        # Sum of doubled areas.
        total_area2 = 0

        ok = True

        # Validate every triangle.
        for i, tri in enumerate(triangles):
            r = right_angle_index(tri)

            # Invalid small triangle.
            if r < 0:
                ok = False
                break

            right_idx[i] = r

            # Every vertex must lie inside the large triangle.
            for p in tri:
                if not inside_large(p, n):
                    ok = False
                    break

            if not ok:
                break

            # Add doubled area.
            total_area2 += abs(cross(tri[0], tri[1], tri[2]))

        if not ok:
            answers.append("NO")
            continue

        # Total doubled area must equal n^2.
        if total_area2 != n * n:
            answers.append("NO")
            continue

        # Convert triangles to integer flat representation.
        tri_int = []

        # Collect all vertices for coordinate compression.
        all_pts_list = []

        for tri in triangles:
            flat = []

            for x, y in tri:
                flat.extend([x, y])
                all_pts_list.append((x, y))

            tri_int.append(flat)

        # Unique sorted vertices.
        all_pts = sorted(set(all_pts_list))

        # Number of unique vertices.
        V = len(all_pts)

        # Point to compressed id.
        point_id = {p: i for i, p in enumerate(all_pts)}

        # Compressed vertex ids for every triangle.
        tri_vid = []

        for flat in tri_int:
            ids = []

            for j in range(3):
                p = (flat[2 * j], flat[2 * j + 1])
                ids.append(point_id[p])

            tri_vid.append(ids)

        # Accumulated angle at every unique vertex.
        angle_at = [0] * V

        # Add direct triangle angle contributions.
        for i in range(m):
            r = right_idx[i]
            ids = tri_vid[i]

            # Right-angle vertex.
            angle_at[ids[r]] += 90

            # Acute vertices.
            angle_at[ids[(r + 1) % 3]] += 45
            angle_at[ids[(r + 2) % 3]] += 45

        # Process T-junction contributions by four direction sweeps.
        for d in range(4):
            events = []

            # Add edge open/close events.
            for flat in tri_int:
                for j in range(3):
                    # Current endpoint.
                    ax = flat[2 * j]
                    ay = flat[2 * j + 1]

                    # Next endpoint.
                    nj = (j + 1) % 3
                    bx = flat[2 * nj]
                    by = flat[2 * nj + 1]

                    # Only process edges of current direction.
                    if edge_direction(bx - ax, by - ay) != d:
                        continue

                    level_a, pos_a = level_pos(d, ax, ay)
                    level_b, pos_b = level_pos(d, bx, by)

                    lo = min(pos_a, pos_b)
                    hi = max(pos_a, pos_b)

                    # Edge opens at lower endpoint.
                    events.append(pack(level_a, lo, 2, -1))

                    # Edge closes at upper endpoint.
                    events.append(pack(level_a, hi, 0, -1))

            # Add query event for every vertex.
            for vid, (x, y) in enumerate(all_pts):
                level, pos = level_pos(d, x, y)
                events.append(pack(level, pos, 1, vid))

            # Sort by level, then position, then type.
            #
            # Important:
            #   close < query < open
            #
            # This prevents endpoints from being counted as strictly inside.
            events.sort()

            prev_level = None

            # Number of currently active intervals on current line.
            active = 0

            # Sweep events.
            for e in events:
                # Extract level.
                level = (e >> LEVEL_SHIFT) - LEVEL_OFFSET

                # Extract event type.
                typ = (e >> TYPE_SHIFT) & 3

                # Extract vertex id + 1.
                vid_plus_one = e & VID_MASK

                # Reset active count when switching to a new line.
                if level != prev_level:
                    prev_level = level
                    active = 0

                if typ == 0:
                    # Closing edge endpoint.
                    active -= 1
                elif typ == 1:
                    # Query vertex: active intervals strictly contain it.
                    vid = vid_plus_one - 1
                    angle_at[vid] += 180 * active
                else:
                    # Opening edge endpoint.
                    active += 1

        # Final angle verification.
        for vid, (x, y) in enumerate(all_pts):
            if x == 0 and y == 0:
                expected = 90
            elif (x == n and y == 0) or (x == 0 and y == n):
                expected = 45
            elif x == 0 or y == 0 or x + y == n:
                expected = 180
            else:
                expected = 360

            if angle_at[vid] != expected:
                ok = False
                break

        answers.append("YES" if ok else "NO")

    # Print all answers.
    sys.stdout.write("\n".join(answers))


if __name__ == "__main__":
    solve()
