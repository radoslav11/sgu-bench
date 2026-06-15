"""Checker for p490: usage checker.py <in> <out> <ans>; exit 0 = accept.

Each test case asks for an H x W figure of black cells ('.') with exactly N
white spots ('#'). Feasibility (figure vs "Impossible") is deterministic, so
it must agree with the reference answer. When the reference produces a figure,
any valid figure is accepted: it must be H rows of W characters, the black
cells must form a single 8-connected component whose bounding box is exactly
the full H x W rectangle, and the number of finite (not touching the border)
4-connected white components must equal N.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def split_cases(lines):
    """Group nonempty-line blocks separated by blank lines into case blocks."""
    cases = []
    cur = []
    for line in lines:
        if line.strip() == "":
            if cur:
                cases.append(cur)
                cur = []
        else:
            cur.append(line)
    if cur:
        cases.append(cur)

    return cases


def main():
    with open(sys.argv[1]) as f:
        in_lines = f.read().split("\n")
    with open(sys.argv[2]) as f:
        out_raw = f.read().split("\n")
    with open(sys.argv[3]) as f:
        ans_raw = f.read().split("\n")

    it = iter([l for l in in_lines if l.strip() != ""])
    t = int(next(it))
    tests = []
    for _ in range(t):
        h, w, n = map(int, next(it).split())
        tests.append((h, w, n))

    out_cases = split_cases(out_raw)
    ans_cases = split_cases(ans_raw)

    if len(ans_cases) != t:
        reject(f"reference produced {len(ans_cases)} case blocks, expected {t}")
    if len(out_cases) != t:
        reject(f"output has {len(out_cases)} case blocks, expected {t}")

    for idx in range(t):
        h, w, n = tests[idx]
        out_block = out_cases[idx]
        ans_block = ans_cases[idx]

        ref_impossible = (len(ans_block) == 1 and
                          ans_block[0].strip().lower() == "impossible")
        out_impossible = (len(out_block) == 1 and
                          out_block[0].strip().lower() == "impossible")

        if ref_impossible != out_impossible:
            if ref_impossible:
                reject(f"case {idx + 1}: reference says Impossible, "
                       f"output produced a figure")
            else:
                reject(f"case {idx + 1}: a valid figure exists, "
                       f"output says Impossible")

        if ref_impossible:
            continue

        grid = out_block
        if len(grid) != h:
            reject(f"case {idx + 1}: expected {h} rows, got {len(grid)}")
        for r, row in enumerate(grid):
            if len(row) != w:
                reject(f"case {idx + 1}: row {r} has length {len(row)}, "
                       f"expected {w}")
            for c in row:
                if c not in (".", "#"):
                    reject(f"case {idx + 1}: bad character '{c}'")

        check_figure(idx + 1, grid, h, w, n)

    sys.exit(0)


def check_figure(case_no, grid, h, w, n):
    black = [(i, j) for i in range(h) for j in range(w) if grid[i][j] == "."]
    if not black:
        reject(f"case {case_no}: figure has no black cells")

    # Single 8-connected component of black cells.
    black_set = set(black)
    start = black[0]
    seen = {start}
    q = deque([start])
    while q:
        i, j = q.popleft()
        for di in (-1, 0, 1):
            for dj in (-1, 0, 1):
                if di == 0 and dj == 0:
                    continue
                nb = (i + di, j + dj)
                if nb in black_set and nb not in seen:
                    seen.add(nb)
                    q.append(nb)
    if len(seen) != len(black_set):
        reject(f"case {case_no}: black cells are not 8-connected "
               f"(one figure required)")

    # Bounding box of the figure must be exactly H x W.
    minr = min(i for i, _ in black)
    maxr = max(i for i, _ in black)
    minc = min(j for _, j in black)
    maxc = max(j for _, j in black)
    if (maxr - minr + 1) != h or (maxc - minc + 1) != w:
        reject(f"case {case_no}: figure bounding box is "
               f"{maxr - minr + 1}x{maxc - minc + 1}, expected {h}x{w}")

    # Count finite white spots: 4-connected white components not touching
    # the border of the H x W rectangle.
    visited = [[False] * w for _ in range(h)]
    spots = 0
    for si in range(h):
        for sj in range(w):
            if grid[si][sj] != "#" or visited[si][sj]:
                continue
            comp = []
            touches_border = False
            visited[si][sj] = True
            q = deque([(si, sj)])
            while q:
                i, j = q.popleft()
                comp.append((i, j))
                if i == 0 or i == h - 1 or j == 0 or j == w - 1:
                    touches_border = True
                for di, dj in ((-1, 0), (1, 0), (0, -1), (0, 1)):
                    ni, nj = i + di, j + dj
                    if 0 <= ni < h and 0 <= nj < w and not visited[ni][nj] \
                            and grid[ni][nj] == "#":
                        visited[ni][nj] = True
                        q.append((ni, nj))
            if not touches_border:
                spots += 1

    if spots != n:
        reject(f"case {case_no}: figure has {spots} spots, expected {n}")


if __name__ == "__main__":
    main()
