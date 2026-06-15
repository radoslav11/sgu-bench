"""Checker for p409: usage checker.py <in> <out> <ans>; exit 0 = accept.

A valid flag is any (N^2)x(N^2) grid of '*'/'.' with exactly K stars in
every row, every column, and every aligned NxN block, so the contestant
grid is validated against the input only. "NO SOLUTION" is deterministic
and must match the reference verdict (in fact a flag always exists for
0 <= K <= N^2, so the reference never prints it).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n, k = map(int, open(sys.argv[1]).read().split())
    out_lines = [
        line.strip() for line in open(sys.argv[2]).read().split("\n")
    ]
    out_lines = [line for line in out_lines if line]
    ans_text = open(sys.argv[3]).read().strip()

    ref_no_solution = ans_text.upper().startswith("NO SOLUTION")
    got_no_solution = (
        " ".join(out_lines).upper().startswith("NO SOLUTION")
    )

    if ref_no_solution != got_no_solution:
        if ref_no_solution:
            reject("expected NO SOLUTION")
        reject("said NO SOLUTION but a flag exists")
    if got_no_solution:
        sys.exit(0)

    size = n * n
    if len(out_lines) != size:
        reject(f"expected {size} lines, got {len(out_lines)}")
    for i, line in enumerate(out_lines):
        if len(line) != size:
            reject(f"line {i + 1} has length {len(line)}, expected {size}")
        if any(ch not in "*." for ch in line):
            reject(f"line {i + 1} contains characters other than '*'/'.'")

    for i, line in enumerate(out_lines):
        if line.count("*") != k:
            reject(f"row {i + 1} has {line.count('*')} stars, expected {k}")
    for j in range(size):
        col = sum(1 for i in range(size) if out_lines[i][j] == "*")
        if col != k:
            reject(f"column {j + 1} has {col} stars, expected {k}")
    for br in range(n):
        for bc in range(n):
            cnt = sum(
                out_lines[br * n + i][bc * n + j] == "*"
                for i in range(n)
                for j in range(n)
            )
            if cnt != k:
                reject(
                    f"block ({br + 1},{bc + 1}) has {cnt} stars, "
                    f"expected {k}"
                )

    sys.exit(0)


if __name__ == "__main__":
    main()
