import sys

INF = 10**30

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    # Read N, M, H, S, E
    n = int(next(it))
    m = int(next(it))
    h = int(next(it))
    s = int(next(it))
    e = int(next(it))

    # Build adjacency list (1-indexed)
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(next(it))
        b = int(next(it))
        t = int(next(it))
        r = int(next(it))
        p = int(next(it))
        # undirected
        adj[a].append((b, t, r, p))
        adj[b].append((a, t, r, p))

    # Read exits
    exits = [int(next(it)) for _ in range(e)]

    def check(x: int):
        """
        Returns (ok, path)
        ok: True if reachable to any exit under max heat limit x
        path: one reconstructed path if ok else []
        """
        dist = [INF] * (n + 1)        # earliest arrival times
        parent = [-1] * (n + 1)       # for path reconstruction
        used = [False] * (n + 1)      # Dijkstra settled set

        dist[s] = 0

        # O(N^2) Dijkstra (N <= 100)
        for _ in range(n):
            u = -1
            best = INF
            # pick unvisited node with smallest dist
            for v in range(1, n + 1):
                if not used[v] and dist[v] < best:
                    best = dist[v]
                    u = v

            if u == -1:               # nothing reachable
                break

            used[u] = True

            # relax edges out of u if allowed by heat constraint
            du = dist[u]
            for to, t, r, p in adj[u]:
                # Heat at corridor end time (du + t):
                heat = r + (du + t) * p
                if heat <= x:
                    nd = du + t
                    if nd < dist[to]:
                        dist[to] = nd
                        parent[to] = u

        # Choose first reachable exit in input order
        exit_node = -1
        for ex in exits:
            if dist[ex] < INF:
                exit_node = ex
                break

        if exit_node == -1:
            return False, []

        # Reconstruct path S -> exit
        path = []
        cur = exit_node
        while cur != -1:
            path.append(cur)
            cur = parent[cur]
        path.reverse()
        return True, path

    # Binary search minimal feasible X in [0, H]
    lo, hi = 0, h
    best_x = -1
    best_path = []

    while lo <= hi:
        mid = (lo + hi) // 2
        ok, path = check(mid)
        if ok:
            best_x = mid
            best_path = path
            hi = mid - 1
        else:
            lo = mid + 1

    out = []
    if best_x == -1:
        out.append("NO\n")
    else:
        out.append("YES\n")
        out.append(f"{best_x}\n")
        out.append(str(len(best_path)) + " " + " ".join(map(str, best_path)) + "\n")

    sys.stdout.write("".join(out))

if __name__ == "__main__":
    solve()
