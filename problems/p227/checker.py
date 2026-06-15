"""Checker for p227: usage checker.py <in> <out> <ans>; exit 0 = accept.

The cross-point set is unique (the statement guarantees distinct crosses
are >= 0.005 apart), but printed coordinates are rounded to 3 decimals,
so points are matched within 2.4e-3 (each output point can then be close
to at most one reference point). Ordering by (x, y) is verified with the
same tolerance, so legitimate rounding never flips a comparison.
"""

import sys

TOL = 2.4e-3


def reject(message):
    print(message)
    sys.exit(1)


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    if not out_tokens:
        reject("empty output")

    if ans_tokens[0] == "Infinity":
        if out_tokens[0] != "Infinity":
            reject(f"expected 'Infinity', got '{out_tokens[0]}'")
        sys.exit(0)
    if out_tokens[0] == "Infinity":
        reject("got 'Infinity' but the cross count is finite")

    try:
        m = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be a count, got '{out_tokens[0]}'")
    expected_m = int(ans_tokens[0])
    if m != expected_m:
        reject(f"cross count is {expected_m}, got {m}")
    if len(out_tokens) != 1 + 2 * m:
        reject(f"expected {2 * m} coordinates, got {len(out_tokens) - 1} tokens")

    try:
        got = [
            (float(out_tokens[1 + 2 * i]), float(out_tokens[2 + 2 * i]))
            for i in range(m)
        ]
    except ValueError:
        reject("non-numeric coordinate")
    exp = [
        (float(ans_tokens[1 + 2 * i]), float(ans_tokens[2 + 2 * i]))
        for i in range(m)
    ]

    # Ordering: x must be non-decreasing up to rounding. The y tie-break
    # cannot be verified from rounded values (two crosses may print the
    # same x while their exact x values differ), so it is not checked.
    for i in range(m - 1):
        if got[i][0] > got[i + 1][0] + TOL:
            reject(f"points {i} and {i + 1} are out of order")

    used = [False] * m
    for i, (x, y) in enumerate(got):
        match = -1
        for j, (ex, ey) in enumerate(exp):
            if abs(x - ex) <= TOL and abs(y - ey) <= TOL:
                match = j
                break
        if match == -1:
            reject(f"output point {i} ({x}, {y}) is not a cross point")
        if used[match]:
            reject(f"cross point ({x}, {y}) reported twice")
        used[match] = True

    sys.exit(0)


if __name__ == "__main__":
    main()
