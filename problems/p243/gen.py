"""Tests for p243 (Broken Chessboard): 1 <= N <= 5, 20x20 table.

An N x N board is partitioned into K rook-connected pieces by merging
random adjacent unit cells; each piece is rotated by a random multiple of
90 degrees and scattered on the 20x20 table without overlaps. Seed 1 is
N=1, seeds 2-3 are N=5 with many distinct pieces / 25 unit pieces, later
seeds vary N and the piece-size distribution.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def partition(n, k):
    # merge adjacent regions of the n x n board until exactly k remain
    region = {(r, c): r * n + c for r in range(n) for c in range(n)}
    count = n * n
    edges = [
        ((r, c), (r + dr, c + dc))
        for r in range(n)
        for c in range(n)
        for dr, dc in ((0, 1), (1, 0))
        if r + dr < n and c + dc < n
    ]
    rng.shuffle(edges)
    for a, b in edges:
        if count == k:
            break
        ra, rb = region[a], region[b]
        if ra != rb:
            for cell in region:
                if region[cell] == rb:
                    region[cell] = ra
            count -= 1
    pieces = {}
    for cell, rid in region.items():
        pieces.setdefault(rid, []).append(cell)
    return list(pieces.values())


def rotate_random(cells):
    for _ in range(rng.randrange(4)):
        cells = [(c, -r) for r, c in cells]
    mr = min(r for r, _ in cells)
    mc = min(c for _, c in cells)
    return sorted((r - mr, c - mc) for r, c in cells)


def scatter(pieces):
    table = [["."] * 20 for _ in range(20)]
    for idx, cells in enumerate(pieces):
        letter = chr(ord("A") + idx)
        cells = rotate_random(cells)
        h = max(r for r, _ in cells) + 1
        w = max(c for _, c in cells) + 1
        while True:
            r0 = rng.randint(0, 20 - h)
            c0 = rng.randint(0, 20 - w)
            spots = [(r0 + r, c0 + c) for r, c in cells]
            if all(table[r][c] == "." for r, c in spots):
                for r, c in spots:
                    table[r][c] = letter
                break
    return table


def emit(n, k):
    pieces = partition(n, k)
    rng.shuffle(pieces)
    table = scatter(pieces)
    print(n)
    print("\n".join("".join(row) for row in table))


if seed == 1:
    emit(1, 1)
elif seed == 2:
    emit(5, 8)
elif seed == 3:
    emit(5, 25)
elif seed == 4:
    emit(5, 2)
elif seed == 5:
    emit(5, 12)
elif seed == 6:
    emit(4, 6)
elif seed == 7:
    emit(2, 3)
elif seed == 8:
    emit(5, 7)
elif seed == 9:
    emit(3, 4)
else:
    n = rng.randint(1, 5)
    emit(n, rng.randint(1, n * n))
