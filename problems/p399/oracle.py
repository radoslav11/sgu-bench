from collections import deque
import sys

INF = 10**9

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    N = int(next(it))
    M = int(next(it))
    grid = [list(next(it).strip()) for _ in range(N)]

    # 4-neighborhood directions
    DR = (-1, 1, 0, 0)
    DC = (0, 0, -1, 1)

    def in_bounds(r: int, c: int) -> bool:
        return 0 <= r < N and 0 <= c < M

    # 1) Label crater components (connected components of 'X')
    crater_id = [[-1] * M for _ in range(N)]
    K = 0
    for i in range(N):
        for j in range(M):
            if grid[i][j] != 'X' or crater_id[i][j] != -1:
                continue
            q = deque([(i, j)])
            crater_id[i][j] = K
            while q:
                r, c = q.popleft()
                for d in range(4):
                    nr, nc = r + DR[d], c + DC[d]
                    if in_bounds(nr, nc) and grid[nr][nc] == 'X' and crater_id[nr][nc] == -1:
                        crater_id[nr][nc] = K
                        q.append((nr, nc))
            K += 1

    # 2) Collect ports: '.' cells adjacent to each crater
    ports = [[] for _ in range(K)]
    for r in range(N):
        for c in range(M):
            if grid[r][c] != '.':
                continue
            seen = set()
            for d in range(4):
                nr, nc = r + DR[d], c + DC[d]
                if in_bounds(nr, nc) and grid[nr][nc] == 'X':
                    cid = crater_id[nr][nc]
                    if cid not in seen:
                        seen.add(cid)
                        ports[cid].append((r, c))

    # BFS over '.' cells with parent pointers for path reconstruction.
    # sources: list of starting '.' cells
    # start_dist: distance assigned to sources
    def bfs(sources, start_dist):
        dist = [[INF] * M for _ in range(N)]
        par = [[(-1, -1)] * M for _ in range(N)]  # parent cell; (-1,-1) means start
        q = deque()

        for (r, c) in sources:
            if dist[r][c] == INF:
                dist[r][c] = start_dist
                q.append((r, c))

        while q:
            r, c = q.popleft()
            for d in range(4):
                nr, nc = r + DR[d], c + DC[d]
                if in_bounds(nr, nc) and grid[nr][nc] == '.' and dist[nr][nc] == INF:
                    dist[nr][nc] = dist[r][c] + 1
                    par[nr][nc] = (r, c)
                    q.append((nr, nc))
        return dist, par

    # Ocean sources are border '.' cells
    ocean_sources = []
    for r in range(N):
        for c in range(M):
            if grid[r][c] == '.' and (r == 0 or r == N - 1 or c == 0 or c == M - 1):
                ocean_sources.append((r, c))

    dist_o, par_o = bfs(ocean_sources, 1)

    # Distances from each crater (multi-source from its ports)
    dist_c = []
    for k in range(K):
        d, _ = bfs(ports[k], 1)
        dist_c.append(d)

    # Pick best port of crater k according to distance matrix d
    def best_port(k, d):
        best = (-1, -1)
        best_val = INF
        for (r, c) in ports[k]:
            if d[r][c] < best_val:
                best_val = d[r][c]
                best = (r, c)
        return best

    # Trace path by following parent pointers until (-1,-1), return set of visited cells
    def trace_path(start, par):
        path = set()
        r, c = start
        while r != -1:
            path.add((r, c))
            r, c = par[r][c]
        return path

    # Candidate (a): two separate pipes (choose two smallest crater->ocean costs)
    cost = [INF] * K
    for k in range(K):
        for (r, c) in ports[k]:
            if dist_o[r][c] < cost[k]:
                cost[k] = dist_o[r][c]

    order = list(range(K))
    order.sort(key=lambda x: cost[x])

    S_sep = set()
    cost_sep = INF
    if K >= 2 and cost[order[0]] < INF and cost[order[1]] < INF:
        pa = best_port(order[0], dist_o)
        pb = best_port(order[1], dist_o)
        S_sep |= trace_path(pa, par_o)
        S_sep |= trace_path(pb, par_o)
        cost_sep = len(S_sep)

    # Candidate (b): one branched component. Enumerate pair of craters and meeting cell m.
    min_multi = INF
    best_i = best_j = -1
    best_m = (-1, -1)

    for i in range(K):
        for j in range(i + 1, K):
            for r in range(N):
                for c in range(M):
                    if grid[r][c] != '.':
                        continue
                    if dist_o[r][c] >= INF or dist_c[i][r][c] >= INF or dist_c[j][r][c] >= INF:
                        continue
                    v = dist_o[r][c] + (dist_c[i][r][c] - 1) + (dist_c[j][r][c] - 1)
                    if v < min_multi:
                        min_multi = v
                        best_i, best_j = i, j
                        best_m = (r, c)

    S_multi = set()
    cost_multi = INF
    if best_i != -1:
        # Ocean path (m -> ocean) using ocean BFS parents
        S_multi |= trace_path(best_m, par_o)

        # BFS from meeting point to reconstruct paths to crater ports
        dist_m, par_m = bfs([best_m], 0)

        pi = best_port(best_i, dist_m)
        pj = best_port(best_j, dist_m)

        S_multi |= trace_path(pi, par_m)
        S_multi |= trace_path(pj, par_m)

        cost_multi = len(S_multi)

    # Choose best solution and apply it
    chosen = S_sep if cost_sep <= cost_multi else S_multi
    for (r, c) in chosen:
        grid[r][c] = '*'

    out = "\n".join("".join(row) for row in grid)
    sys.stdout.write(out + "\n")

if __name__ == "__main__":
    solve()
