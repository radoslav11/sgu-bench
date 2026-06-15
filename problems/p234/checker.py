"""Checker for p234: usage checker.py <in> <out> <ans>; exit 0 = accept.

The guardian count is the unique optimum and must match the reference.
The placement is free-form: the grid must mark removed cells '#', place
exactly K guardians on present cells with no two on edge-adjacent cells,
and every present cell must be a guardian or edge-adjacent to one.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    m, n = int(in_tokens[0]), int(in_tokens[1])
    grid = [
        [int(in_tokens[2 + i * n + j]) for j in range(n)] for i in range(m)
    ]

    expected_k = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the guardian count, got '{out_tokens[0]}'")
    if k != expected_k:
        reject(f"optimal count is {expected_k}, got {k}")

    rows = out_tokens[1:]
    if len(rows) != m:
        reject(f"expected {m} grid rows, got {len(rows)}")

    placed = 0
    for i in range(m):
        if len(rows[i]) != n:
            reject(f"row {i + 1} has length {len(rows[i])}, expected {n}")
        for j, ch in enumerate(rows[i]):
            if ch not in ".G#":
                reject(f"row {i + 1}: invalid character '{ch}'")
            if (ch == "#") != (grid[i][j] == 0):
                reject(f"cell ({i + 1}, {j + 1}): '#' must mark removed cells")
            if ch == "G":
                placed += 1

    if placed != k:
        reject(f"declared {k} guardians, placed {placed}")

    for i in range(m):
        for j in range(n):
            if grid[i][j] == 0:
                continue
            covered = rows[i][j] == "G"
            for di, dj in ((-1, 0), (1, 0), (0, -1), (0, 1)):
                ni, nj = i + di, j + dj
                if 0 <= ni < m and 0 <= nj < n and rows[ni][nj] == "G":
                    if rows[i][j] == "G":
                        reject(
                            f"guardians at ({i + 1}, {j + 1}) and "
                            f"({ni + 1}, {nj + 1}) kill each other"
                        )
                    covered = True
            if not covered:
                reject(f"cell ({i + 1}, {j + 1}) is not defended")

    sys.exit(0)


if __name__ == "__main__":
    main()
