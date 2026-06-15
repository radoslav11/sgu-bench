import sys
import math


def min_speed_squared_for_jump(d1, d2, b, t, g):
    """
    Compute the minimum required speed squared for one jump.

    The frog:
    - starts at height 0,
    - crosses the wall after horizontal distance d1,
    - lands at height 0 after another horizontal distance d2,
    - must cross the wall at some height h in [b, t].

    The derived formula is:

        v^2(h) = A / h + B * h

    where:

        A = g * d1 * d2 / 2
        B = g * (d1 + d2)^2 / (2 * d1 * d2)

    The unconstrained minimizing height is:

        h = d1 * d2 / (d1 + d2)

    Then we clamp h to the hole interval [b, t].
    """

    # Coefficient A.
    A = g * d1 * d2 / 2.0

    # Coefficient B.
    B = g * (d1 + d2) * (d1 + d2) / (2.0 * d1 * d2)

    # Best crossing height without hole restrictions.
    h0 = d1 * d2 / (d1 + d2)

    # Clamp crossing height into the allowed hole interval.
    h = max(b, min(t, h0))

    # Return the minimum possible speed squared.
    return A / h + B * h


def solve_case(b1, t1, b2, t2, l, ds, df, g):
    """
    Solve one test case.
    """

    def cost(p):
        """
        Required maximum speed squared if the frog lands p units
        after the first wall.
        """

        # First jump:
        # start -> first wall distance is ds,
        # first wall -> landing point distance is p.
        first = min_speed_squared_for_jump(ds, p, b1, t1, g)

        # Second jump:
        # landing point -> second wall distance is l - p,
        # second wall -> final point distance is df.
        second = min_speed_squared_for_jump(l - p, df, b2, t2, g)

        # The frog's maximal speed must handle both jumps.
        return max(first, second)

    # The landing point must be between the walls.
    lo = 0.0
    hi = l

    # Ternary search on the unimodal function cost(p).
    # 200 iterations are already more than enough for double precision.
    for _ in range(200):
        # First trisection point.
        m1 = lo + (hi - lo) / 3.0

        # Second trisection point.
        m2 = hi - (hi - lo) / 3.0

        # Discard the third that cannot contain the minimum.
        if cost(m1) < cost(m2):
            hi = m2
        else:
            lo = m1

    # Approximate optimal landing point.
    p = (lo + hi) / 2.0

    # cost(p) is speed squared, so return square root.
    return math.sqrt(cost(p))


def main():
    # Read all real numbers from input.
    data = list(map(float, sys.stdin.read().split()))

    # Each test case consists of exactly eight numbers.
    out = []

    for i in range(0, len(data), 8):
        # Extract one test case.
        b1, t1, b2, t2, l, ds, df, g = data[i:i + 8]

        # Solve it.
        ans = solve_case(b1, t1, b2, t2, l, ds, df, g)

        # Six digits after decimal are enough for 1e-4 accuracy.
        out.append(f"{ans:.6f}")

    # Print all answers.
    print("\n".join(out))


if __name__ == "__main__":
    main()
