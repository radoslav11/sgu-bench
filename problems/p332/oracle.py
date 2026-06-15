"""Independent oracle for p332: Chebyshev center of a convex polygon.

The largest inscribed circle is the LP: maximize r subject to
n_i . c - r >= n_i . a_i for every edge (a_i, b_i) with unit inward
normal n_i (polygon is CCW). Solved exactly with scipy linprog.
"""

import math
import sys

import numpy as np
from scipy.optimize import linprog


def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    pts = [
        (float(data[1 + 2 * i]), float(data[2 + 2 * i])) for i in range(n)
    ]

    a_ub = np.zeros((n, 3))
    b_ub = np.zeros(n)
    for i in range(n):
        ax, ay = pts[i]
        bx, by = pts[(i + 1) % n]
        dx, dy = bx - ax, by - ay
        norm = math.hypot(dx, dy)
        nx, ny = -dy / norm, dx / norm  # inward normal for CCW order
        # n . c - r >= n . a  ->  -n . c + r <= -n . a
        a_ub[i] = (-nx, -ny, 1.0)
        b_ub[i] = -(nx * ax + ny * ay)

    res = linprog(
        c=[0.0, 0.0, -1.0],
        A_ub=a_ub,
        b_ub=b_ub,
        bounds=[(None, None), (None, None), (0, None)],
        method="highs",
    )
    print("%.6f" % res.x[2])


if __name__ == "__main__":
    main()
