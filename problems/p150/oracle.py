import sys
from math import gcd


def main():
    x1, y1, x2, y2, n = map(int, sys.stdin.read().split())

    dx = x2 - x1
    dy = y2 - y1

    # Coordinates are integers, so the start point is a lattice corner. Any
    # axis-aligned (or degenerate) segment rides a grid line and never enters
    # a cell interior.
    if dx == 0 or dy == 0:
        print("no solution")
        return

    sx = 1 if dx > 0 else -1
    sy = 1 if dy > 0 else -1

    # First cell entered just after leaving the lattice corner (x1, y1).
    cx = x1 if sx > 0 else x1 - 1
    cy = y1 if sy > 0 else y1 - 1

    adx, ady = abs(dx), abs(dy)
    visited = 1
    while visited < n:
        # Exact next-crossing comparison: t_x = nx_off/adx vs t_y = ny_off/ady
        # where nx_off = |next vertical line - x1| etc., all integers.
        nx_line = cx + 1 if sx > 0 else cx
        ny_line = cy + 1 if sy > 0 else cy
        tx_num = (nx_line - x1) * sx  # in units of 1/adx
        ty_num = (ny_line - y1) * sy  # in units of 1/ady

        lhs = tx_num * ady
        rhs = ty_num * adx
        t_at_end = adx * ady  # t == 1 in common units
        if lhs < rhs:
            if lhs >= t_at_end:
                break
            cx += sx
        elif rhs < lhs:
            if rhs >= t_at_end:
                break
            cy += sy
        else:
            # Exact lattice-corner crossing: skip diagonally.
            if lhs >= t_at_end:
                break
            cx += sx
            cy += sy
        visited += 1

    if visited == n:
        print(cx, cy)
    else:
        print("no solution")


if __name__ == "__main__":
    main()
