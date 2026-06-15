import sys
from collections import deque

def solve() -> None:
    data = list(map(int, sys.stdin.buffer.read().split()))
    if not data:
        return
    it = iter(data)

    q = next(it)          # number of rows (x dimension)
    c = next(it)          # number of cols (y dimension)
    n = next(it)          # number of cultures
    t = next(it)          # number of seconds to simulate/expand

    starts = [(next(it), next(it)) for _ in range(n)]  # starting positions

    # owner[x][y] = culture index owning cell (1..n), 0 if empty.
    # Use 1-based indexing by allocating (q+1) x (c+1).
    owner = [[0] * (c + 1) for _ in range(q + 1)]

    # Multi-source BFS queue items: (x, y, dist, culture)
    bfs = deque()

    # Seed the BFS in increasing culture index order so ties go to smaller index.
    for i, (x, y) in enumerate(starts, start=1):
        # Input guarantees unique in-grid positions, but keep checks anyway.
        if 1 <= x <= q and 1 <= y <= c and owner[x][y] == 0:
            owner[x][y] = i
            bfs.append((x, y, 0, i))

    # 4-direction moves
    dirs = ((1, 0), (-1, 0), (0, 1), (0, -1))

    # BFS expansion up to distance t
    while bfs:
        x, y, dist, culture = bfs.popleft()

        # If we've already spent t seconds to reach here, no further expansion allowed.
        if dist >= t:
            continue

        nd = dist + 1
        for dx, dy in dirs:
            nx, ny = x + dx, y + dy
            # Claim only if within bounds and currently empty.
            if 1 <= nx <= q and 1 <= ny <= c and owner[nx][ny] == 0:
                owner[nx][ny] = culture
                bfs.append((nx, ny, nd, culture))

    # Count owned cells for each culture
    ans = [0] * (n + 1)
    for x in range(1, q + 1):
        row = owner[x]
        for y in range(1, c + 1):
            ans[row[y]] += 1

    # Output counts for cultures 1..n
    out = "\n".join(str(ans[i]) for i in range(1, n + 1))
    sys.stdout.write(out)

if __name__ == "__main__":
    solve()
