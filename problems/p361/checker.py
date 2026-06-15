"""Checker for p361: usage checker.py <in> <out> <ans>; exit 0 = accept.

The flag itself may be any grid of '0'/'#' where every 3x2 and 2x3
rectangle contains exactly two '#', as long as the number of '#' cells
equals the reference's minimum. The reference grid is never compared
cell-by-cell.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n, m = map(int, open(sys.argv[1]).read().split())
    out_lines = open(sys.argv[2]).read().split()
    ans_lines = open(sys.argv[3]).read().split()

    if not out_lines:
        reject("empty output")

    # The reference always finds a flag, so "No solution" is never correct.
    if len(out_lines) != n:
        reject(f"expected {n} rows, got {len(out_lines)}")
    for i, row in enumerate(out_lines):
        if len(row) != m:
            reject(f"row {i + 1} has length {len(row)}, expected {m}")
        if any(ch not in "0#" for ch in row):
            reject(f"row {i + 1} contains characters other than '0'/'#'")

    grid = out_lines

    def blue(i, j):
        return 1 if grid[i][j] == "#" else 0

    for i in range(n - 2):
        for j in range(m - 1):
            s = sum(blue(i + di, j + dj) for di in range(3) for dj in range(2))
            if s != 2:
                reject(f"3x2 rectangle at ({i + 1},{j + 1}) has {s} blue cells")

    for i in range(n - 1):
        for j in range(m - 2):
            s = sum(blue(i + di, j + dj) for di in range(2) for dj in range(3))
            if s != 2:
                reject(f"2x3 rectangle at ({i + 1},{j + 1}) has {s} blue cells")

    got_count = sum(row.count("#") for row in grid)
    ref_count = sum(row.count("#") for row in ans_lines)
    if got_count != ref_count:
        reject(f"{got_count} blue cells used, optimum is {ref_count}")

    sys.exit(0)


if __name__ == "__main__":
    main()
