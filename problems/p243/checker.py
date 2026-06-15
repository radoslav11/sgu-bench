"""Checker for p243: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any valid reconstruction is accepted: the output must be an N x N board
using exactly the letters from the table, and the cells of each letter
must equal that piece's shape up to a rotation by 0/90/180/270 degrees.
Shape equality also forces the piece to stay connected and fully used.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def normalize(cells):
    mr = min(r for r, _ in cells)
    mc = min(c for _, c in cells)
    return tuple(sorted((r - mr, c - mc) for r, c in cells))


def rotations(cells):
    shapes = set()
    cur = list(cells)
    for _ in range(4):
        shapes.add(normalize(cur))
        cur = [(c, -r) for r, c in cur]
    return shapes


def main():
    in_lines = open(sys.argv[1]).read().split()
    n = int(in_lines[0])
    table = in_lines[1:21]

    pieces = {}
    for r in range(20):
        for c in range(20):
            ch = table[r][c]
            if ch != ".":
                pieces.setdefault(ch, []).append((r, c))

    out_lines = [ln.strip() for ln in open(sys.argv[2]).read().split()]
    if len(out_lines) != n:
        reject(f"expected {n} board lines, got {len(out_lines)}")
    for ln in out_lines:
        if len(ln) != n:
            reject(f"board line '{ln}' is not {n} characters")

    placed = {}
    for r in range(n):
        for c in range(n):
            ch = out_lines[r][c]
            placed.setdefault(ch, []).append((r, c))

    if "." in placed:
        reject("board contains empty cells")
    if set(placed) != set(pieces):
        reject(
            f"letters used {sorted(placed)} differ from pieces {sorted(pieces)}"
        )

    for ch, cells in placed.items():
        if normalize(cells) not in rotations(pieces[ch]):
            reject(f"piece {ch} does not match any rotation of its shape")

    sys.exit(0)


if __name__ == "__main__":
    main()
