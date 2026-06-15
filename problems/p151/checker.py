"""Checker for p151: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any triangle with |AB| = c, |AC| = b and median |AM| = m (M midpoint of
BC) within 1e-3 absolute error is accepted; coordinates must not exceed
10^4 in absolute value. A "Mission impossible" claim is deterministic and
must match the reference answer.
"""

import math
import sys

EPS = 1e-3


def reject(message):
    print(message)
    sys.exit(1)


def main():
    c, b, m = map(float, open(sys.argv[1]).read().split())
    out_text = open(sys.argv[2]).read()
    ans_text = open(sys.argv[3]).read()

    ans_impossible = "impossible" in ans_text.lower()
    out_lower = " ".join(out_text.lower().split())

    if "impossible" in out_lower:
        if out_lower != "mission impossible":
            reject(f"garbled impossibility claim: '{out_text.strip()}'")
        if not ans_impossible:
            reject("claimed 'Mission impossible' but a triangle exists")
        sys.exit(0)

    tokens = out_text.split()
    if len(tokens) != 6:
        reject(f"expected 6 coordinates, got {len(tokens)} tokens")
    try:
        ax, ay, bx, by, cx, cy = map(float, tokens)
    except ValueError:
        reject("non-numeric coordinate")

    for v in (ax, ay, bx, by, cx, cy):
        if not math.isfinite(v) or abs(v) > 1e4 + 1e-6:
            reject(f"coordinate {v} out of allowed range [-1e4, 1e4]")

    mx, my = (bx + cx) / 2, (by + cy) / 2
    d_ab = math.hypot(bx - ax, by - ay)
    d_ac = math.hypot(cx - ax, cy - ay)
    d_am = math.hypot(mx - ax, my - ay)

    if abs(d_ab - c) > EPS:
        reject(f"|AB| = {d_ab:.6f}, expected {c}")
    if abs(d_ac - b) > EPS:
        reject(f"|AC| = {d_ac:.6f}, expected {b}")
    if abs(d_am - m) > EPS:
        reject(f"|AM| = {d_am:.6f}, expected {m}")

    sys.exit(0)


if __name__ == "__main__":
    main()
