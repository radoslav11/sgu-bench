import sys
import math


# Direction arrays indexed by grid digit:
# 0 = still, 1 = up, 2 = down, 3 = right, 4 = left.
FX = [0, 0, 0, 1, -1]
FY = [0, -1, 1, 0, 0]


def solve():
    data = sys.stdin.read().strip().split()

    if not data:
        return

    ptr = 0

    # Read lake dimensions.
    n = int(data[ptr])
    ptr += 1
    m = int(data[ptr])
    ptr += 1

    # Read grid.
    grid = data[ptr:ptr + n]
    ptr += n

    # Peter start, input is 1-based (x, y).
    peter_x = int(data[ptr])
    ptr += 1
    peter_y = int(data[ptr])
    ptr += 1

    # Boat start, input is 1-based (x, y).
    boat_x = int(data[ptr])
    ptr += 1
    boat_y = int(data[ptr])
    ptr += 1

    # Convert to 0-based row/column.
    pr = peter_y - 1
    pc = peter_x - 1
    br = boat_y - 1
    bc = boat_x - 1

    total = n * m
    INF = 10**100

    def inside(r, c):
        """Return True if cell (r, c) is inside the lake."""
        return 0 <= r < n and 0 <= c < m

    def flow_of(r, c):
        """Return flow vector of cell (r, c)."""
        d = ord(grid[r][c]) - ord('0')
        return FX[d], FY[d]

    # ------------------------------------------------------------
    # 1. Shortest times for Peter to reach every cell center.
    # Distances are stored in half-minutes.
    # ------------------------------------------------------------

    BIG = 10**18
    dist_half = [BIG] * total

    start_id = pr * m + pc
    dist_half[start_id] = 0

    # Dial buckets for weights 1 and 2.
    buckets = [[], [], []]
    buckets[0].append(start_id)

    max_dist = 0
    d = 0

    while d <= max_dist:
        bucket = buckets[d % 3]

        while bucket:
            cell_id = bucket.pop()

            # Stale bucket entry.
            if dist_half[cell_id] != d:
                continue

            r = cell_id // m
            c = cell_id % m

            fx, fy = flow_of(r, c)

            # Try all swimming choices.
            for k in range(5):
                vx = fx + FX[k]
                vy = fy + FY[k]

                # Zero velocity means Peter stays at the center.
                if vx == 0 and vy == 0:
                    continue

                # Direction to neighboring center.
                ox = (vx > 0) - (vx < 0)
                oy = (vy > 0) - (vy < 0)

                nr = r + oy
                nc = c + ox

                if not inside(nr, nc):
                    continue

                # Speed 2 means 0.5 minutes, otherwise 1 minute.
                w = 1 if abs(vx) == 2 or abs(vy) == 2 else 2

                nd = d + w
                nid = nr * m + nc

                if nd < dist_half[nid]:
                    dist_half[nid] = nd
                    buckets[nd % 3].append(nid)
                    if nd > max_dist:
                        max_dist = nd

        d += 1

    # Convert half-minutes to minutes.
    dist = [INF] * total
    for i in range(total):
        if dist_half[i] != BIG:
            dist[i] = dist_half[i] * 0.5

    ans = INF

    # ------------------------------------------------------------
    # 2. Try direct escape to the shore.
    # ------------------------------------------------------------

    for r in range(n):
        for c in range(m):
            cell_id = r * m + c

            if dist[cell_id] >= INF:
                continue

            fx, fy = flow_of(r, c)
            best_exit = INF

            for k in range(5):
                vx = fx + FX[k]
                vy = fy + FY[k]

                if vx == 0 and vy == 0:
                    continue

                tx = INF
                ty = INF

                # Time to vertical border.
                if vx > 0:
                    tx = ((m - 1) + 0.5 - c) / vx
                elif vx < 0:
                    tx = (c + 0.5) / (-vx)

                # Time to horizontal border.
                if vy > 0:
                    ty = ((n - 1) + 0.5 - r) / vy
                elif vy < 0:
                    ty = (r + 0.5) / (-vy)

                t_wall = min(tx, ty)

                # Time to next center in that direction.
                ox = (vx > 0) - (vx < 0)
                oy = (vy > 0) - (vy < 0)

                nr = r + oy
                nc = c + ox

                if inside(nr, nc):
                    t_center = 0.5 if abs(vx) == 2 or abs(vy) == 2 else 1.0
                else:
                    t_center = INF

                # Peter reaches shore before needing to recalculate velocity.
                if t_wall <= t_center + 1e-12:
                    best_exit = min(best_exit, t_wall)

            if best_exit < INF:
                ans = min(ans, dist[cell_id] + best_exit)

    # ------------------------------------------------------------
    # 3. Simulate boat path.
    # ------------------------------------------------------------

    bcell = []
    btime = []
    seen = [-1] * total

    cr = br
    cc = bc
    bt = 0

    cycle_start = -1
    period = 0
    stationary = False

    while True:
        cell_id = cr * m + cc

        # Repeated cell means the deterministic path entered a cycle.
        if seen[cell_id] != -1:
            cycle_start = seen[cell_id]
            period = bt - btime[cycle_start]
            break

        seen[cell_id] = len(bcell)
        bcell.append(cell_id)
        btime.append(bt)

        fx, fy = flow_of(cr, cc)

        # Still water: boat remains here forever.
        if fx == 0 and fy == 0:
            stationary = True
            break

        nr = cr + fy
        nc = cc + fx

        # Boat reaches shore and leaves grid.
        if not inside(nr, nc):
            break

        cr = nr
        cc = nc
        bt += 1

    length = len(bcell)

    def is_cyclic(i):
        """Return True if boat record index i belongs to the cycle."""
        return cycle_start != -1 and i >= cycle_start

    # ------------------------------------------------------------
    # 4. Meet boat at cell centers.
    # ------------------------------------------------------------

    for i in range(length):
        cell_id = bcell[i]

        if dist[cell_id] >= INF:
            continue

        base = float(btime[i])

        if stationary and i == length - 1:
            # Boat stays here after base time.
            ans = min(ans, max(dist[cell_id], base))

        elif is_cyclic(i):
            # First cycle visit not earlier than Peter's arrival.
            cand = base

            if dist[cell_id] > base:
                k = math.ceil((dist[cell_id] - base) / period - 1e-9)
                if k < 0:
                    k = 0
                cand = base + k * period

            ans = min(ans, cand)

        else:
            # Prefix cell is visited once.
            if dist[cell_id] <= base + 1e-9:
                ans = min(ans, base)

    # ------------------------------------------------------------
    # 5. Meet boat inside an edge.
    # ------------------------------------------------------------

    def edge_meet(a_id, db_idx, tb, cyc):
        """Try meeting boat while it moves from A to A' at time tb."""
        nonlocal ans

        ar = a_id // m
        ac = a_id % m

        dx = FX[db_idx]
        dy = FY[db_idx]

        a2r = ar + dy
        a2c = ac + dx

        # If boat edge goes out of lake, direct shore escape handles saving.
        if not inside(a2r, a2c):
            return

        a2 = a2r * m + a2c

        # Case 1: Peter comes head-on from A' toward A.
        if dist[a2] < INF:
            f2x, f2y = flow_of(a2r, a2c)

            sp = 0

            # Flow at A' helps Peter move back toward A.
            if f2x == -dx and f2y == -dy:
                sp = 2

            # Still water at A', Peter can swim toward A with speed 1.
            elif f2x == 0 and f2y == 0:
                sp = 1

            if sp > 0:
                da = dist[a2]

                lo = da - 1.0
                hi = da + 1.0 / sp

                t = float(tb)

                # If this is a cycle edge, advance to the first valid repetition.
                if cyc and t < lo:
                    k = math.ceil((lo - t) / period - 1e-9)
                    if k < 0:
                        k = 0
                    t += k * period

                if t >= lo - 1e-9 and t <= hi + 1e-9:
                    meet_time = (1.0 + t + sp * da) / (sp + 1)
                    ans = min(ans, meet_time)

        # Case 2: Peter chases from A in the same direction with speed 2.
        if dist[a_id] < INF:
            fax, fay = flow_of(ar, ac)

            # To have speed 2 in the boat direction, flow must point that way.
            if fax == dx and fay == dy:
                da = dist[a_id]

                lo = da - 0.5
                hi = da

                t = float(tb)

                # For cycle edge, advance to first valid repeated traversal.
                if cyc and t < lo:
                    k = math.ceil((lo - t) / period - 1e-9)
                    if k < 0:
                        k = 0
                    t += k * period

                if t >= lo - 1e-9 and t <= hi + 1e-9:
                    meet_time = 2.0 * da - t
                    ans = min(ans, meet_time)

    # Process all explicit consecutive boat edges.
    for i in range(length - 1):
        cell_id = bcell[i]
        r = cell_id // m
        c = cell_id % m
        dchar = ord(grid[r][c]) - ord('0')

        edge_meet(cell_id, dchar, btime[i], is_cyclic(i))

    # If boat trajectory has a cycle, the last recorded cell also has
    # an edge back into the cycle.
    if cycle_start != -1:
        last = length - 1
        cell_id = bcell[last]
        r = cell_id // m
        c = cell_id % m
        dchar = ord(grid[r][c]) - ord('0')

        edge_meet(cell_id, dchar, btime[last], True)

    # ------------------------------------------------------------
    # Output answer.
    # ------------------------------------------------------------

    if ans >= INF:
        print("SOS")
    else:
        print(f"{ans:.2f}")


if __name__ == "__main__":
    solve()
