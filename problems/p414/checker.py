"""Checker for p414: usage checker.py <in> <out> <ans>; exit 0 = accept.

The reference answer is one of: "-1" (no orthogonal circle), "-2" (infinitely
many), or three reals "x y r" (the unique circle). The -1 / -2 verdicts are
deterministic and must match the reference. When the answer is a circle it is
unique, so the contestant's circle must equal the reference's within 1e-6
relative-or-absolute error; as an extra guard it must also be orthogonal to
every input circle.
"""

import sys
import math


def reject(message):
    print(message)
    sys.exit(1)


def close(a, b, tol=1e-6):
    return abs(a - b) <= tol + tol * max(abs(a), abs(b))


def main():
    in_tok = open(sys.argv[1]).read().split()
    out_tok = open(sys.argv[2]).read().split()
    ans_tok = open(sys.argv[3]).read().split()

    n = int(in_tok[0])
    circ = []
    p = 1
    for _ in range(n):
        circ.append((int(in_tok[p]), int(in_tok[p + 1]), int(in_tok[p + 2])))
        p += 3

    if not ans_tok:
        reject("empty reference answer")
    if not out_tok:
        reject("empty output")

    ref_kind = ans_tok[0]

    if ref_kind in ("-1", "-2"):
        if len(out_tok) != 1 or out_tok[0] != ref_kind:
            reject(f"expected '{ref_kind}', got '{' '.join(out_tok)}'")
        sys.exit(0)

    # Reference is a circle (three reals).
    if len(ans_tok) < 3:
        reject("malformed reference answer")
    rx, ry, rr = float(ans_tok[0]), float(ans_tok[1]), float(ans_tok[2])

    if len(out_tok) != 3:
        reject(f"expected three reals, got '{' '.join(out_tok)}'")
    try:
        x, y, r = float(out_tok[0]), float(out_tok[1]), float(out_tok[2])
    except ValueError:
        reject(f"non-numeric output '{' '.join(out_tok)}'")

    if r <= 0:
        reject(f"radius must be positive, got {r}")

    if not (close(x, rx) and close(y, ry) and close(r, rr)):
        reject(
            f"circle ({x},{y},{r}) differs from reference ({rx},{ry},{rr})"
        )

    # Orthogonality guard: d^2 == r^2 + ri^2 for every input circle.
    for (xi, yi, ri) in circ:
        d2 = (x - xi) ** 2 + (y - yi) ** 2
        lhs = d2
        rhs = r * r + ri * ri
        if abs(lhs - rhs) > 1e-3 + 1e-6 * max(abs(lhs), abs(rhs)):
            reject(
                f"not orthogonal to circle ({xi},{yi},{ri}): "
                f"d^2={lhs} vs r^2+ri^2={rhs}"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
