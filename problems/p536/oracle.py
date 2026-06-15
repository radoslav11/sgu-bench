from collections import deque

def solve():
    n, m = map(int, input().split())
    tbl = [input().strip() for _ in range(n)]

    pieces = []   # list of (r,c) for each black piece
    ptype = []    # 'K','B','R' for each piece (black knight/bishop/rook)
    king_r = king_c = -1

    # Parse the board: find king and black pieces
    for i in range(n):
        for j in range(m):
            ch = tbl[i][j]
            if ch == '*':
                king_r, king_c = i, j
            elif ch in "KBR":
                pieces.append((i, j))
                ptype.append(ch)

    k = len(pieces)
    if k == 0:
        print(0)
        return

    # attack[r][c] will store a list of indices of pieces that attack (r,c).
    # Using list-of-lists (instead of set) is fine; duplicates won't happen here.
    attack = [[[] for _ in range(m)] for _ in range(n)]

    # Precompute all attacked squares for each black piece, based on initial board.
    for idx, ((ci, cj), typ) in enumerate(zip(pieces, ptype)):
        if typ == 'K':  # black knight
            for di in range(-2, 3):
                for dj in range(-2, 3):
                    if abs(di) + abs(dj) == 3:  # (2,1) / (1,2)
                        ni, nj = ci + di, cj + dj
                        if 0 <= ni < n and 0 <= nj < m:
                            attack[ni][nj].append(idx)

        elif typ == 'B':  # bishop: 4 diagonal rays
            dirs = [(1, 1), (1, -1), (-1, 1), (-1, -1)]
            for di, dj in dirs:
                step = 1
                while True:
                    ni, nj = ci + di * step, cj + dj * step
                    if not (0 <= ni < n and 0 <= nj < m):
                        break
                    attack[ni][nj].append(idx)
                    # Stop ray when hitting a non-empty square
                    if tbl[ni][nj] != '.':
                        break
                    step += 1

        else:  # 'R' rook: 4 orthogonal rays
            dirs = [(1, 0), (-1, 0), (0, 1), (0, -1)]
            for di, dj in dirs:
                step = 1
                while True:
                    ni, nj = ci + di * step, cj + dj * step
                    if not (0 <= ni < n and 0 <= nj < m):
                        break
                    attack[ni][nj].append(idx)
                    if tbl[ni][nj] != '.':
                        break
                    step += 1

    # Map piece position -> index to quickly set capture bit
    pos_to_idx = {pos: i for i, pos in enumerate(pieces)}

    # dist[r][c][mask] = min moves, initialized to -1 (unvisited)
    # Sizes: n * m * (1<<k)
    dist = [[[-1] * (1 << k) for _ in range(m)] for _ in range(n)]

    start_mask = 0
    dist[king_r][king_c][start_mask] = 0
    q = deque([(king_r, king_c, start_mask)])

    # 8 king moves
    dr = [-1, -1, -1,  0, 0, 1, 1, 1]
    dc = [-1,  0,  1, -1, 1,-1, 0, 1]

    full = (1 << k) - 1

    while q:
        r, c, mask = q.popleft()
        curd = dist[r][c][mask]

        if mask == full:
            print(curd)
            return

        for t in range(8):
            nr, nc = r + dr[t], c + dc[t]
            if not (0 <= nr < n and 0 <= nc < m):
                continue

            # Check if (nr,nc) is attacked by any alive piece
            attacked = False
            for idx in attack[nr][nc]:
                if (mask & (1 << idx)) == 0:  # piece still alive
                    attacked = True
                    break
            if attacked:
                continue

            nmask = mask
            # If we moved onto a black piece square, we capture it
            if (nr, nc) in pos_to_idx:
                nmask |= 1 << pos_to_idx[(nr, nc)]

            if dist[nr][nc][nmask] == -1:
                dist[nr][nc][nmask] = curd + 1
                q.append((nr, nc, nmask))

    print(-1)


if __name__ == "__main__":
    solve()
