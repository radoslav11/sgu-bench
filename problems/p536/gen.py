"""Tests for p536 (Berland Chess): n,m <= 15, <= 15 pieces total, exactly
one king '*' not under attack initially.

The BFS state is (king cell, captured mask), so cost peaks when there are
many black pieces (mask up to 2^14) on a large board with long forced
detours around attacked squares. Seed 1 is the minimal board (king only).
Seeds 2 and 3 are maximal: 15x15 with 14 black pieces, packed so the king
must thread between attack zones. Later seeds randomize size, piece mix,
and density, including some unreachable-piece boards (answer -1).

The generator mirrors the reference's attack rules so it only emits boards
where the king starts safe; placements that fail are rejected and retried.
"""

import random
import sys

MAXN = 15
MAX_PIECES = 15  # including the king


def attacked_cells(board, n, m):
    """Set of cells attacked by the black pieces, blockers respected."""
    attacked = set()
    knight = [(2, 1), (2, -1), (-2, 1), (-2, -1),
              (1, 2), (1, -2), (-1, 2), (-1, -2)]
    diag = [(1, 1), (1, -1), (-1, 1), (-1, -1)]
    ortho = [(1, 0), (-1, 0), (0, 1), (0, -1)]

    for i in range(n):
        for j in range(m):
            ch = board[i][j]
            if ch == "K":
                for di, dj in knight:
                    ni, nj = i + di, j + dj
                    if 0 <= ni < n and 0 <= nj < m:
                        attacked.add((ni, nj))
            elif ch in "BR":
                rays = diag if ch == "B" else ortho
                for di, dj in rays:
                    ni, nj = i + di, j + dj
                    while 0 <= ni < n and 0 <= nj < m:
                        attacked.add((ni, nj))
                        if board[ni][nj] != ".":
                            break
                        ni, nj = ni + di, nj + dj

    return attacked


def build_board(n, m, n_black, rng):
    """Place n_black pieces + king so the king starts un-attacked, or None."""
    for _ in range(200):
        cells = [(i, j) for i in range(n) for j in range(m)]
        rng.shuffle(cells)
        if n_black + 1 > len(cells):
            return None

        board = [["." for _ in range(m)] for _ in range(n)]
        chosen = cells[: n_black + 1]
        for (i, j) in chosen[:n_black]:
            board[i][j] = rng.choice("KBR")

        king = chosen[n_black]
        if king in attacked_cells(board, n, m):
            continue
        board[king[0]][king[1]] = "*"
        return ["".join(row) for row in board]

    return None


def emit(n, m, board):
    print(n, m)
    print("\n".join(board))


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(1, 1, ["*"])
    sys.exit()

def spread_knight_board(n, m, spots):
    # Place up to MAX_PIECES-1 knights on the given lattice and drop the king
    # on the first safe empty cell; returns the emitted board rows or None.
    board = [["." for _ in range(m)] for _ in range(n)]
    placed = 0
    for (r, c) in spots:
        if placed >= MAX_PIECES - 1:
            break
        board[r][c] = "K"
        placed += 1
    attacked = attacked_cells(board, n, m)
    for i in range(n):
        for j in range(m):
            if board[i][j] == "." and (i, j) not in attacked:
                board[i][j] = "*"
                return ["".join(row) for row in board]
    return None


if seed == 2:
    # Maximal solvable board: 14 black knights spread on a sparse lattice so
    # all are capturable and the king must traverse the whole board, forcing
    # the full 2^14 captured-mask BFS (the worst case for the state space).
    n = m = MAXN
    spots = [(r, c) for r in range(0, n, 3) for c in range(0, m, 3)]
    board = spread_knight_board(n, m, spots)
    emit(n, m, board)
    sys.exit()

if seed == 3:
    # Second maximal solvable board: 14 knights on a different lattice so the
    # attack zones overlap differently while staying fully capturable.
    n = m = MAXN
    spots = [(r, c) for r in range(0, n, 2) for c in range(0, m, 4)]
    board = spread_knight_board(n, m, spots)
    emit(n, m, board)
    sys.exit()

n = rng.randint(2, MAXN)
m = rng.randint(2, MAXN)
n_black = rng.randint(1, min(MAX_PIECES - 1, n * m - 1))
board = build_board(n, m, n_black, rng)
while board is None:
    n = rng.randint(2, MAXN)
    m = rng.randint(2, MAXN)
    n_black = rng.randint(1, min(MAX_PIECES - 1, n * m - 1))
    board = build_board(n, m, n_black, rng)

emit(n, m, board)
