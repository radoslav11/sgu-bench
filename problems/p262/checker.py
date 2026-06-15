"""Checker for p262: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal pixel count is deterministic and must match the reference's
first number. The grid is any set of exactly that many pixels such that
every pair of symbols differs on at least one chosen pixel. The
reference's own grid is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, k = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    rows = in_tokens[3:]
    symbols = []
    for i in range(k):
        grid = "".join(rows[i * n:(i + 1) * n])
        symbols.append(grid)

    best = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        claimed = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if claimed != best:
        reject(f"minimal count is {best}, got {claimed}")

    grid_rows = out_tokens[1:]
    if len(grid_rows) != n:
        reject(f"expected {n} grid rows, got {len(grid_rows)}")
    for i, row in enumerate(grid_rows):
        if len(row) != m or any(c not in "01" for c in row):
            reject(f"row {i + 1} is not {m} chars of 0/1")

    chosen = [i for i, c in enumerate("".join(grid_rows)) if c == "1"]
    if len(chosen) != best:
        reject(f"set has {len(chosen)} pixels, claimed {claimed}")

    for a in range(k):
        for b in range(a + 1, k):
            if all(symbols[a][p] == symbols[b][p] for p in chosen):
                reject(f"symbols {a + 1} and {b + 1} are not distinguished")

    sys.exit(0)


if __name__ == "__main__":
    main()
