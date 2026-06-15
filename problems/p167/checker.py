"""Checker for p167: usage checker.py <in> <out> <ans>; exit 0 = accept.

The oil total X is deterministic and must match the reference. The K
listed cells are a certificate: they must be distinct in-grid cells whose
oil sums to X and which form a convex polyomino (occupied rows are
consecutive, every row and every column is a contiguous run, consecutive
row intervals overlap) - exactly the "two movement directions suffice"
shapes. Any such set is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_header(tokens):
    """Return (X, index of first cell token); accepts 'Oil : X' / 'Oil: X'."""
    if not tokens:
        reject("empty output")
    if tokens[0].lower() == "oil" and len(tokens) >= 3 and tokens[1] == ":":
        return int(tokens[2]), 3
    if tokens[0].lower() == "oil:" and len(tokens) >= 2:
        return int(tokens[1]), 2
    reject(f"output must start with 'Oil : X', got {' '.join(tokens[:3])!r}")


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, k = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    grid = [[int(in_tokens[3 + i * m + j]) for j in range(m)]
            for i in range(n)]

    try:
        x_got, idx = parse_header(out_tokens)
    except ValueError:
        reject("non-integer oil value")
    x_ref, _ = parse_header(ans_tokens)
    if x_got != x_ref:
        reject(f"optimal oil is {x_ref}, claimed {x_got}")

    rest = out_tokens[idx:]
    if len(rest) != 2 * k:
        reject(f"expected {k} coordinate pairs, got {len(rest)} tokens")
    try:
        nums = [int(t) for t in rest]
    except ValueError:
        reject("non-integer coordinate")

    cells = set()
    total = 0
    for i in range(k):
        r, c = nums[2 * i], nums[2 * i + 1]
        if not (1 <= r <= n and 1 <= c <= m):
            reject(f"cell ({r},{c}) out of grid")
        if (r, c) in cells:
            reject(f"cell ({r},{c}) listed twice")
        cells.add((r, c))
        total += grid[r - 1][c - 1]
    if total != x_got:
        reject(f"cells sum to {total}, not the claimed {x_got}")

    if k == 0:
        sys.exit(0)

    rows = sorted({r for r, _ in cells})
    if rows[-1] - rows[0] + 1 != len(rows):
        reject("occupied rows are not consecutive")

    intervals = {}
    for r in rows:
        cols = sorted(c for rr, c in cells if rr == r)
        if cols[-1] - cols[0] + 1 != len(cols):
            reject(f"row {r} is not a contiguous run")
        intervals[r] = (cols[0], cols[-1])

    for r in rows[:-1]:
        l1, r1 = intervals[r]
        l2, r2 = intervals[r + 1]
        if min(r1, r2) < max(l1, l2):
            reject(f"rows {r} and {r + 1} do not overlap")

    for c in sorted({c for _, c in cells}):
        rs = sorted(r for r, cc in cells if cc == c)
        if rs[-1] - rs[0] + 1 != len(rs):
            reject(f"column {c} is not a contiguous run")

    sys.exit(0)


if __name__ == "__main__":
    main()
