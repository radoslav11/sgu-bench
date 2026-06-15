"""Checker for p399: usage checker.py <in> <out> <ans>; exit 0 = accept.

The pipe square count is the (deterministic) objective: it must equal
the reference's count. The map itself is a certificate: crater cells
must be untouched, pipe cells '*' may only replace '.', and water from
the surrounding ocean must reach at least two distinct craters flowing
through edge-adjacent '*' cells (a '*' cell on the border touches the
ocean; a crater is reached when a watered '*' cell shares an edge with
it). Craters are 4-connected components of 'X'.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_lines = open(sys.argv[1]).read().split()
    out_lines = open(sys.argv[2]).read().split()
    ans_lines = open(sys.argv[3]).read().split()

    n, m = int(in_lines[0]), int(in_lines[1])
    grid = in_lines[2 : 2 + n]
    ans_grid = ans_lines[:n]

    if len(out_lines) != n:
        reject(f"expected {n} map rows, got {len(out_lines)}")
    for i, row in enumerate(out_lines):
        if len(row) != m:
            reject(f"row {i + 1}: expected {m} characters, got {len(row)}")
        for j, ch in enumerate(row):
            if grid[i][j] == "X":
                if ch != "X":
                    reject(f"crater cell ({i + 1},{j + 1}) was modified")
            elif ch not in ".*":
                reject(f"bad character '{ch}' at ({i + 1},{j + 1})")

    used = sum(row.count("*") for row in out_lines)
    optimal = sum(row.count("*") for row in ans_grid)
    if used != optimal:
        reject(f"used {used} pipe squares, optimum is {optimal}")

    # crater labels: 4-connected components of 'X'
    label = [[-1] * m for _ in range(n)]
    n_craters = 0
    for i in range(n):
        for j in range(m):
            if grid[i][j] == "X" and label[i][j] == -1:
                q = deque([(i, j)])
                label[i][j] = n_craters
                while q:
                    r, c = q.popleft()
                    for nr, nc in ((r-1, c), (r+1, c), (r, c-1), (r, c+1)):
                        if (0 <= nr < n and 0 <= nc < m
                                and grid[nr][nc] == "X"
                                and label[nr][nc] == -1):
                            label[nr][nc] = n_craters
                            q.append((nr, nc))
                n_craters += 1

    # water spreads from the ocean through '*' cells
    watered = [[False] * m for _ in range(n)]
    q = deque()
    for i in range(n):
        for j in range(m):
            if (out_lines[i][j] == "*"
                    and (i in (0, n - 1) or j in (0, m - 1))):
                watered[i][j] = True
                q.append((i, j))
    reached = set()
    while q:
        r, c = q.popleft()
        for nr, nc in ((r-1, c), (r+1, c), (r, c-1), (r, c+1)):
            if not (0 <= nr < n and 0 <= nc < m):
                continue
            if grid[nr][nc] == "X":
                reached.add(label[nr][nc])
            elif out_lines[nr][nc] == "*" and not watered[nr][nc]:
                watered[nr][nc] = True
                q.append((nr, nc))

    if len(reached) < 2:
        reject(f"only {len(reached)} crater(s) connected to the ocean")

    sys.exit(0)


if __name__ == "__main__":
    main()
