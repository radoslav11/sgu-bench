import sys
import threading
def main():
    import sys
    sys.setrecursionlimit(10000)
    input = sys.stdin.readline

    n = int(input())
    a = [list(map(int, input().split())) for _ in range(n)]

    # Build a list of all edges: (weight, row, col)
    edges = []
    for i in range(n):
        for j in range(n):
            edges.append((a[i][j], i, j))
    edges.sort(key=lambda x: x[0])

    # Hopcroft-Karp implementation
    from collections import deque
    INF = 10**9

    def hopcroft_karp(adj, N, M):
        pairU = [-1]*N   # pairU[u] = matched v for u, or -1
        pairV = [-1]*M   # pairV[v] = matched u for v, or -1
        dist = [0]*N

        def bfs():
            queue = deque()
            for u in range(N):
                if pairU[u] < 0:
                    dist[u] = 0
                    queue.append(u)
                else:
                    dist[u] = INF
            found = False
            while queue:
                u = queue.popleft()
                for v in adj[u]:
                    pu = pairV[v]
                    if pu < 0:
                        found = True
                    elif dist[pu] == INF:
                        dist[pu] = dist[u] + 1
                        queue.append(pu)
            return found

        def dfs(u):
            for v in adj[u]:
                pu = pairV[v]
                if pu < 0 or (dist[pu] == dist[u] + 1 and dfs(pu)):
                    pairU[u] = v
                    pairV[v] = u
                    return True
            dist[u] = INF
            return False

        matching = 0
        while bfs():
            for u in range(N):
                if pairU[u] < 0 and dfs(u):
                    matching += 1
        return matching, pairU

    # Build adjacency up to index mid
    def build_adj(mid):
        adj = [[] for _ in range(n)]
        for i in range(mid+1):
            _, u, v = edges[i]
            adj[u].append(v)
        return adj

    # Binary search for minimal mid that allows perfect matching
    low, high, ans = 0, n*n-1, n*n-1
    while low <= high:
        mid = (low + high)//2
        adj = build_adj(mid)
        match_sz, _ = hopcroft_karp(adj, n, n)
        if match_sz == n:
            ans = mid
            high = mid - 1
        else:
            low = mid + 1

    # Rebuild final matching
    adj = build_adj(ans)
    _, pairU = hopcroft_karp(adj, n, n)

    # Output result
    threshold = edges[ans][0]
    out = [str(threshold)]
    for i in range(n):
        # pairU[i] is the column matched to row i
        out.append(f"{i+1} {pairU[i]+1}")
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
