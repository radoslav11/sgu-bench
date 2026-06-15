"""Checker for p244: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO part must match the reference. For "YES" any triangle A, B, C
is accepted whose height AH, bisector AD and median AM (all from vertex A)
are within 1e-6 of the given lengths, whose area is non-zero, and whose
coordinates stay within 1e8 in absolute value.
"""

import math
import sys

TOL = 1e-6 + 1e-9


def reject(message):
    print(message)
    sys.exit(1)


def main():
    h, d, m = map(int, open(sys.argv[1]).read().split())
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    if len(out_tokens) != 7:
        reject(f"expected 6 coordinates after YES, got {len(out_tokens) - 1}")
    try:
        ax, ay, bx, by, cx, cy = map(float, out_tokens[1:])
    except ValueError:
        reject("coordinates are not numbers")
    for v in (ax, ay, bx, by, cx, cy):
        if not math.isfinite(v) or abs(v) > 1e8:
            reject(f"coordinate {v} out of allowed range")

    cross = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax)
    if cross == 0:
        reject("triangle has zero area")
    bc = math.hypot(cx - bx, cy - by)
    if bc == 0:
        reject("B and C coincide")

    height = abs(cross) / bc
    if abs(height - h) > TOL:
        reject(f"height {height} differs from {h} by more than 1e-6")

    median = math.hypot((bx + cx) / 2 - ax, (by + cy) / 2 - ay)
    if abs(median - m) > TOL:
        reject(f"median {median} differs from {m} by more than 1e-6")

    ab = math.hypot(bx - ax, by - ay)
    ac = math.hypot(cx - ax, cy - ay)
    t = ab / (ab + ac)
    dx = bx + (cx - bx) * t
    dy = by + (cy - by) * t
    bisector = math.hypot(dx - ax, dy - ay)
    if abs(bisector - d) > TOL:
        reject(f"bisector {bisector} differs from {d} by more than 1e-6")

    sys.exit(0)


if __name__ == "__main__":
    main()
