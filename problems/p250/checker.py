"""Checker for p250: usage checker.py <in> <out> <ans>; exit 0 = accept.

The maximal area (first token) must match the reference answer; -1 means
no house exists and nothing else is required. Otherwise the printed grid
must equal the input grid except that some empty cells become 8, and the
8-cells must form three vertically stacked rectangles: contiguous rows,
shared left column, each block of constant width and height >= 1, with
the middle block strictly narrower than both the upper and lower blocks.
Any plan achieving the optimal area is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    grid = in_tokens[2 : 2 + n * m]
    expected_area = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        got_area = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if got_area != expected_area:
        reject(f"optimal area is {expected_area}, got {got_area}")

    if expected_area == -1:
        sys.exit(0)

    cells = out_tokens[1:]
    if len(cells) != n * m:
        reject(f"expected {n * m} grid tokens after area, got {len(cells)}")

    rows = {}  # row -> list of columns containing 8
    count8 = 0
    for i, (orig, new) in enumerate(zip(grid, cells)):
        if new == "8":
            if orig != "0":
                reject(f"house cell {i // m},{i % m} is not empty in the input")
            rows.setdefault(i // m, []).append(i % m)
            count8 += 1
        elif new != orig:
            reject(f"non-house cell {i // m},{i % m} changed from {orig} to {new}")

    if count8 != expected_area:
        reject(f"area claims {expected_area} but {count8} cells are marked 8")

    rlist = sorted(rows)
    if rlist[-1] - rlist[0] + 1 != len(rlist):
        reject("house rows are not contiguous")

    lefts = set()
    widths = []
    for r in rlist:
        cols = sorted(rows[r])
        if cols[-1] - cols[0] + 1 != len(cols):
            reject(f"house cells in row {r} are not contiguous")
        lefts.add(cols[0])
        widths.append(len(cols))
    if len(lefts) != 1:
        reject("left edges of the blocks are not on one vertical")

    h = len(widths)
    ok = False
    for a in range(1, h - 1):
        for b in range(a + 1, h):
            wu, wm, wl = widths[:a], widths[a:b], widths[b:]
            if (
                len(set(wu)) == 1
                and len(set(wm)) == 1
                and len(set(wl)) == 1
                and wm[0] < wu[0]
                and wm[0] < wl[0]
            ):
                ok = True
                break
        if ok:
            break
    if not ok:
        reject("8-cells do not form upper/middle/lower blocks with narrow middle")

    sys.exit(0)


if __name__ == "__main__":
    main()
