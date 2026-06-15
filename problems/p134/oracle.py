import sys
sys.setrecursionlimit(1000000)

def main():
    data = sys.stdin.read().split()
    # Parse input
    it = iter(data)
    n = int(next(it))
    adj = [[] for _ in range(n+1)]
    for _ in range(n-1):
        u = int(next(it)); v = int(next(it))
        adj[u].append(v)
        adj[v].append(u)

    # Compute subtree sizes by DFS
    tr_sz = [0]*(n+1)
    def dfs(u, p):
        """Return size of subtree rooted at u, excluding parent p."""
        tr_sz[u] = 1
        for v in adj[u]:
            if v == p:
                continue
            dfs(v, u)
            tr_sz[u] += tr_sz[v]
        return tr_sz[u]

    dfs(1, 0)

    # Compute for each node the largest component size after removal
    val = [0]*(n+1)
    mn = n
    for u in range(1, n+1):
        # Component size of the “rest of the tree”
        max_comp = n - tr_sz[u]
        # For each neighbor v that is a child in the DFS tree,
        # its subtree becomes one component when removing u
        for v in adj[u]:
            if tr_sz[v] < tr_sz[u]:
                max_comp = max(max_comp, tr_sz[v])
        val[u] = max_comp
        if max_comp < mn:
            mn = max_comp

    # Collect centroids
    centroids = [u for u in range(1, n+1) if val[u] == mn]

    # Output results
    # First line: minimum largest component size and count of centroids
    print(mn, len(centroids))
    # Second line: sorted list of centroids
    print(*centroids)

if __name__ == "__main__":
    main()
