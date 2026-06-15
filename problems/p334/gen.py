"""Tests for p334 (Tiny Puzzle): a connected 9-cell polyomino, cropped to
its bounding box (so H, W <= 9 and H + W <= 10).

Seed 1 is the already-solved 3x3 square (K=1). Seed 2 is the 1x9 line,
seed 3 the 9x1 line, seed 4 a diagonal staircase (many parts needed),
seed 5 a plus-shape. Remaining seeds grow random polyominoes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(cells):
    min_r = min(r for r, c in cells)
    min_c = min(c for r, c in cells)
    cells = {(r - min_r, c - min_c) for r, c in cells}
    h = max(r for r, c in cells) + 1
    w = max(c for r, c in cells) + 1
    print(h, w)
    for r in range(h):
        print("".join("X" if (r, c) in cells else "." for c in range(w)))


if seed == 1:
    emit([(r, c) for r in range(3) for c in range(3)])
elif seed == 2:
    emit([(0, c) for c in range(9)])
elif seed == 3:
    emit([(r, 0) for r in range(9)])
elif seed == 4:
    # staircase: pairs stepping diagonally
    emit([(0, 0), (0, 1), (1, 1), (1, 2), (2, 2), (2, 3), (3, 3), (3, 4), (4, 4)])
elif seed == 5:
    emit([(0, 2), (1, 2), (2, 0), (2, 1), (2, 2), (2, 3), (2, 4), (3, 2), (4, 2)])
else:
    cells = {(0, 0)}
    while len(cells) < 9:
        r, c = rng.choice(sorted(cells))
        dr, dc = rng.choice([(0, 1), (0, -1), (1, 0), (-1, 0)])
        cells.add((r + dr, c + dc))
    emit(cells)
