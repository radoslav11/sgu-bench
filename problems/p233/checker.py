"""Checker for p233: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any point C on the circle achieving the maximal angle ACB is accepted.
For each case: C must lie on the circle (1e-4 absolute tolerance), and
after projecting C exactly onto the circle, the angle ACB must be within
1e-6 radians of the angle achieved by the reference point in <ans>.
"""

import math
import sys


def reject(message):
    print(message)
    sys.exit(1)


def angle(cx, cy, x0, y0, r, xa, ya, xb, yb):
    # project (cx, cy) onto the circle to neutralize radial rounding
    dx, dy = cx - x0, cy - y0
    d = math.hypot(dx, dy)
    if d < 1e-12:
        return -1.0
    cx, cy = x0 + dx * r / d, y0 + dy * r / d
    ax, ay = xa - cx, ya - cy
    bx, by = xb - cx, yb - cy
    na, nb = math.hypot(ax, ay), math.hypot(bx, by)
    if na < 1e-12 or nb < 1e-12:
        return -1.0
    cosv = max(-1.0, min(1.0, (ax * bx + ay * by) / (na * nb)))
    return math.acos(cosv)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    if len(out_tokens) != 2 * n:
        reject(f"expected {2 * n} numbers, got {len(out_tokens)}")
    if len(ans_tokens) != 2 * n:
        reject(f"answer file malformed: {len(ans_tokens)} tokens")

    for i in range(n):
        x0, y0, r, xa, ya, xb, yb = map(
            int, in_tokens[1 + 7 * i:8 + 7 * i]
        )
        try:
            cx, cy = float(out_tokens[2 * i]), float(out_tokens[2 * i + 1])
        except ValueError:
            reject(f"case {i + 1}: non-numeric coordinates")
        if not (math.isfinite(cx) and math.isfinite(cy)):
            reject(f"case {i + 1}: non-finite coordinates")

        if abs(math.hypot(cx - x0, cy - y0) - r) > 1e-4:
            reject(f"case {i + 1}: point ({cx}, {cy}) is not on the circle")

        rx, ry = float(ans_tokens[2 * i]), float(ans_tokens[2 * i + 1])
        got = angle(cx, cy, x0, y0, r, xa, ya, xb, yb)
        best = angle(rx, ry, x0, y0, r, xa, ya, xb, yb)
        if got < best - 1e-6:
            reject(
                f"case {i + 1}: angle {got:.9f} < optimal {best:.9f}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
