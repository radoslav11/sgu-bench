import sys

DIRS = [(-1, -1), (-1, 0), (-1, 1),
        ( 0, -1),          ( 0, 1),
        ( 1, -1), ( 1, 0), ( 1, 1)]

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    board = [list(next(it)) for _ in range(n)]

    # Locate the queen and treat its start as empty for movement purposes.
    sx = sy = 0
    for i in range(n):
        for j in range(n):
            if board[i][j] == 'Q':
                sx, sy = i, j
                board[i][j] = '.'

    # dist[x][y][p] = minimum moves to reach (x,y) with parity p, else -1.
    dist = [[[-1, -1] for _ in range(n)] for __ in range(n)]

    # Buckets for BFS layers by exact move count (0..m).
    buckets = [[] for _ in range(m + 1)]

    dist[sx][sy][0] = 0
    buckets[0].append((sx, sy))

    # Process distances 0..m-1 and relax to d+1 and d+2 where possible.
    for d in range(m):
        p1 = (d + 1) & 1  # parity after 1 move
        p2 = d & 1        # parity after 2 moves (same parity)

        for x, y in buckets[d]:
            # Skip stale entries (if later we found a shorter distance of same parity).
            if dist[x][y][d & 1] != d:
                continue

            for dx, dy in DIRS:
                # Check if an immediate bounce is possible from the starting square
                # along this direction (requires the behind cell to exist and not be 'W').
                bx, by = x - dx, y - dy
                can_bounce = (0 <= bx < n and 0 <= by < n and board[bx][by] != 'W')

                try1 = True
                try2 = (d + 2 <= m)

                xx, yy = x + dx, y + dy
                # Scan along the ray while within bounds and at least one relaxation is active.
                while (try1 or try2) and 0 <= xx < n and 0 <= yy < n:
                    cell = board[xx][yy]

                    # White piece blocks completely.
                    if cell == 'W':
                        break

                    # Relax distance d+1
                    if try1:
                        if dist[xx][yy][p1] == -1:
                            dist[xx][yy][p1] = d + 1
                            buckets[d + 1].append((xx, yy))
                        elif dist[xx][yy][p1] <= d - 1:
                            # Prune: further squares won't benefit for d+1 relaxations.
                            try1 = False

                    # Relax distance d+2 if bounce is possible
                    if try2 and can_bounce:
                        if dist[xx][yy][p2] == -1:
                            dist[xx][yy][p2] = d + 2
                            buckets[d + 2].append((xx, yy))
                        elif dist[xx][yy][p2] <= d:
                            # Prune for d+2 relaxations.
                            try2 = False

                    # After moving at least one step, we can bounce by returning
                    # to the previous square (since we didn't pass through a 'W').
                    can_bounce = True

                    # Black piece can be landed on, but blocks further scanning.
                    if cell == 'B':
                        break

                    xx += dx
                    yy += dy

    mp = m & 1
    ans = 0
    cnt_any = 0

    for i in range(n):
        for j in range(n):
            if dist[i][j][mp] != -1:
                ans += 1
                cnt_any += 1
            elif dist[i][j][mp ^ 1] != -1:
                cnt_any += 1

    # If m>0 and only starting cell reachable at all, then no legal move exists.
    if m > 0 and cnt_any == 1:
        print(0)
    else:
        print(ans)

if __name__ == "__main__":
    solve()
