import sys
from typing import Tuple

def clamp(v: int, lo: int, hi: int) -> int:
    """Clamp integer v into [lo, hi]."""
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v

def solve_one(n: int, a: Tuple[int, int], b: Tuple[int, int], c: Tuple[int, int]) -> str:
    """
    Solve one instance of the Black-white king problem.

    n: board size (N x N)
    a: (x, y) of first king (call it black)
    b: (x, y) of second king (call it white)
    c: (x, y) of black-white (invisible) king

    Returns the output string with two lines.
    """
    # If black-white king starts on same cell as one of the kings
    # (should not happen with valid input, but we keep the C++ logic)
    if a == c or b == c:
        return "YES\n0\n"

    ax, ay = a
    bx, by = b
    cx, cy = c

    # Distances in x and y between the two visible kings
    dx = abs(ax - bx)
    dy = abs(ay - by)

    # If vertical distance is larger, swap x and y for all three kings
    # to enforce dx >= dy.
    if dx < dy:
        # swap coordinates: (x, y) -> (y, x)
        ax, ay = ay, ax
        bx, by = by, bx
        cx, cy = cy, cx
        # Note: n remains same as board is square
        dx = abs(ax - bx)
        dy = abs(ay - by)

    # Chebyshev distance between the two visible kings
    D = max(dx, dy)

    # Max number of black-white steps we need to check
    max_steps = D // 2 - 1

    # Direction of motion along x for king b relative to king a
    # +1 if b is to the right of a, -1 if to the left (or same x, but then dx=0).
    x_dir = 1 if bx > ax else -1

    # If there's no positive time when interception is possible before meet
    if max_steps <= 0:
        # Minimal number of moves for each of the two visible kings
        # to become neighbors is D - 1.
        return f"NO\n{D - 1}\n"

    # Loop over time steps t = 1..max_steps
    for t in range(1, max_steps + 1):
        # Black-white king's reachable x-range and y-range
        x3 = cx - t
        x4 = cx + t
        y3 = cy - t
        y4 = cy + t

        def check(king_x: int, king_y: int, other_x: int, other_y: int, dir_: int) -> bool:
            """
            Check if a given visible king starting at (king_x, king_y),
            moving along shortest path to (other_x, other_y) with x-direction dir_,
            can be in any cell of the black-white king's reachable square
            at time t.
            """
            # x-position after t steps
            x = king_x + dir_ * t

            # y-position range after t steps, combining:
            #  1) up to t vertical steps
            #  2) constraints imposed by shortest path to (other_x, other_y)
            y_min = max(
                clamp(king_y - t, 1, n),
                clamp(other_y - D + t, 1, n)
            )
            y_max = min(
                clamp(king_y + t, 1, n),
                clamp(other_y + D - t, 1, n)
            )

            # If x is outside [x3, x4], there's no overlap in x
            if x < x3 or x > x4:
                return False

            # If x is exactly on the left or right boundary of black-white's square
            if x == x3 or x == x4:
                # Intersection if vertical ranges intersect
                return max(y_min, y3) <= min(y_max, y4)

            # If x is strictly inside the x-interval:
            # Intersection if there's some y in the overlap of [y_min,y_max] and [y3,y4].
            # Checking if y3 or y4 lies in [y_min, y_max] is sufficient.
            if y_min <= y3 <= y_max:
                return True
            if y_min <= y4 <= y_max:
                return True
            return False

        # Try intercepting king a moving toward b
        if check(ax, ay, bx, by, x_dir):
            return f"YES\n{t}\n"
        # Try intercepting king b moving toward a (opposite x direction)
        if check(bx, by, ax, ay, -x_dir):
            return f"YES\n{t}\n"

    # If for all t <= max_steps interception is impossible,
    # black-white king cannot win.
    return f"NO\n{D - 1}\n"


def main() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    # Single test case as in provided C++ code
    n = int(next(it))
    a = (int(next(it)), int(next(it)))
    b = (int(next(it)), int(next(it)))
    c = (int(next(it)), int(next(it)))

    sys.stdout.write(solve_one(n, a, b, c))


if __name__ == "__main__":
    main()
