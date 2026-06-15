import sys
sys.setrecursionlimit(10000)

def hungarian(cost):
    # cost: list of lists, size n x m, with n <= m
    n, m = len(cost), len(cost[0])
    INF = 10**15
    # potL: u[0..n], potR: v[0..m], p: match for right side, way: back-pointer
    u = [0]*(n+1)
    v = [0]*(m+1)
    p = [0]*(m+1)
    way = [0]*(m+1)
    for i in range(1, n+1):
        p[0] = i
        j0 = 0
        minv = [INF]*(m+1)
        used = [False]*(m+1)
        # Find augmenting path for row i
        while True:
            used[j0] = True
            i0 = p[j0]
            delta = INF
            j1 = 0
            # Try all columns j
            for j in range(1, m+1):
                if not used[j]:
                    cur = cost[i0-1][j-1] - u[i0] - v[j]
                    if cur < minv[j]:
                        minv[j] = cur
                        way[j] = j0
                    if minv[j] < delta:
                        delta = minv[j]
                        j1 = j
            # Update potentials
            for j in range(m+1):
                if used[j]:
                    u[p[j]] += delta
                    v[j] -= delta
                else:
                    minv[j] -= delta
            j0 = j1
            if p[j0] == 0:
                break
        # Augment matching
        while True:
            j1 = way[j0]
            p[j0] = p[j1]
            j0 = j1
            if j0 == 0:
                break
    # p[j] = matched row for column j; duals in u[1..n], v[1..m]
    return u, v, p

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n, m = int(next(it)), int(next(it))
    edges = []
    for _ in range(m):
        u = int(next(it))-1
        v = int(next(it))-1
        w = int(next(it))
        edges.append((u, v, w))
    # Build tree adjacency (first n-1 edges)
    tree_adj = [[] for _ in range(n)]
    for i in range(n-1):
        u,v,_ = edges[i]
        tree_adj[u].append((v,i))
        tree_adj[v].append((u,i))
    # Prepare cost matrix (n-1) x m
    # Cells default to 0 ("leave this tree edge alone" option); the
    # editorial's INF init broke rows not covered by any non-tree edge.
    matrix = [[0]*m for _ in range(n-1)]
    # DFS to mark path
    def dfs(u, parent, target, j_edge):
        if u == target:
            return True
        for v, idx in tree_adj[u]:
            if v == parent: continue
            if dfs(v, u, target, j_edge):
                _,_,wj = edges[j_edge]
                _,_,wi = edges[idx]
                matrix[idx][j_edge] = min(matrix[idx][j_edge], wj - wi)
                return True
        return False
    # Fill matrix for each non-tree edge
    for j in range(n-1, m):
        u, v, _ = edges[j]
        dfs(u, -1, v, j)
    # Run Hungarian to get dual potentials
    u_pot, v_pot, match = hungarian(matrix)
    # Output results
    out = []
    # For tree edges, increase by u_pot[i+1]
    for i in range(n-1):
        u,v,w = edges[i]
        d = w + u_pot[i+1]
        out.append(str(d))
    # For non-tree edges, decrease by v_pot[j+1]
    for j in range(n-1, m):
        u,v,w = edges[j]
        d = w - v_pot[j+1]
        out.append(str(d))
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
