import sys


def solve():
    # Read input.
    # Input order is: x y d2 d1 b
    data = list(map(int, sys.stdin.read().split()))
    x, y, d2, d1, b = data

    # Hex-grid direction vectors.
    vx = [1, 0, -1, -1, 0, 1]
    vy = [0, 1, 1, 0, -1, -1]

    # If already at the target position with the target heading,
    # zero turns are needed.
    if x == 0 and y == 0 and d1 == d2:
        print(0)
        return

    # Large enough upper bound for number of full laps.
    MHI = 6_000_000_000_000

    # Store the best answer over the two possible first directions.
    best = None

    # First move can be:
    # turn = 0: move in direction d1,
    # turn = 1: turn left first, then move in direction d1 + 1.
    for turn in range(2):
        # First direction used.
        fd = (d1 + turn) % 6

        # Minimal number of direction-runs needed so that the final run
        # direction is d2.
        R0 = ((d2 - fd) % 6) + 1

        # c[k] = number of times direction k appears in the base fragment
        # of R0 consecutive runs.
        c = [0] * 6
        for i in range(R0):
            c[(fd + i) % 6] += 1

        # Compute forced displacement from one mandatory move in each run
        # of the base fragment.
        #
        # Extra full laps over all 6 directions have zero displacement,
        # so they do not affect this forced displacement.
        Cx = 0
        Cy = 0
        for k in range(6):
            Cx += c[k] * vx[k]
            Cy += c[k] * vy[k]

        # Remaining displacement to be produced by extra moves.
        Dx = x - Cx
        Dy = y - Cy

        # The function to minimize over variable C is:
        # |Dx + C| + |Dy - C| + |C|.
        #
        # Its unconstrained optimum is median(-Dx, Dy, 0).
        vals = [-Dx, Dy, 0]
        vals.sort()
        Cstar = vals[1]

        def cap(m, k):
            """
            Maximum number of extra moves allowed in direction k.

            Direction k appears m + c[k] times as a run.
            Each run already has one mandatory move and can receive
            at most b - 1 extra moves.
            """
            return (m + c[k]) * (b - 1)

        def evaluate(m):
            """
            For a fixed number of full laps m:
            - determine feasibility,
            - if feasible, compute the minimal total number of moves.
            """

            # Let C be the signed net movement along axis 2:
            # C = s[2] - s[5].
            #
            # The other signed axes are forced:
            # A = Dx + C = s[0] - s[3]
            # B = Dy - C = s[1] - s[4]
            #
            # Apply capacity constraints for all three axes.

            lo = max(
                -cap(m, 5),          # from -cap[5] <= C
                -cap(m, 3) - Dx,     # from -cap[3] <= Dx + C
                Dy - cap(m, 1),      # from Dy - C <= cap[1]
            )

            hi = min(
                cap(m, 2),           # from C <= cap[2]
                cap(m, 0) - Dx,      # from Dx + C <= cap[0]
                Dy + cap(m, 4),      # from -cap[4] <= Dy - C
            )

            # No feasible value of C.
            if lo > hi:
                return False, 0

            # Clamp the unconstrained optimum to the feasible interval.
            C = max(lo, min(hi, Cstar))

            # Minimum number of extra moves.
            extra = abs(Dx + C) + abs(Dy - C) + abs(C)

            # Forced moves are one per run:
            # R0 runs in the base fragment and 6m runs from full laps.
            total = R0 + 6 * m + extra

            return True, total

        # Binary search for the smallest feasible m.
        left = 0
        right = MHI

        while left < right:
            mid = (left + right) // 2

            feasible, _ = evaluate(mid)

            # Feasibility is monotonic in m.
            if feasible:
                right = mid
            else:
                left = mid + 1

        first_feasible = left

        # Now minimize the unimodal/discrete-convex answer function.
        left = first_feasible
        right = MHI

        while left < right:
            mid = (left + right) // 2

            _, value_mid = evaluate(mid)
            _, value_next = evaluate(mid + 1)

            # If moving to mid + 1 does not improve the answer,
            # the optimum is at mid or to the left.
            if value_next >= value_mid:
                right = mid
            else:
                # Otherwise the function is still decreasing.
                left = mid + 1

        # Best value for this initial first direction.
        _, candidate = evaluate(left)

        # Update global best.
        if best is None or candidate < best:
            best = candidate

    print(best)


if __name__ == "__main__":
    solve()
