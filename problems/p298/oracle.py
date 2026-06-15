import sys
from collections import deque

# We'll implement:
# - SCC via Kosaraju
# - check(extra_edges): feasibility + construct assignment
# - binary search on K for edge N -> 1 with weight K (i.e. A1 >= AN + K)

def kosaraju_scc(n, adj, radj):
    """Return comp array of size n with component ids [0..k-1]."""
    sys.setrecursionlimit(1_000_000)

    visited = [False] * n
    order = []

    def dfs1(u):
        visited[u] = True
        for v in adj[u]:
            if not visited[v]:
                dfs1(v)
        order.append(u)

    for i in range(n):
        if not visited[i]:
            dfs1(i)

    comp = [-1] * n
    cid = 0

    def dfs2(u):
        for v in radj[u]:
            if comp[v] == -1:
                comp[v] = cid
                dfs2(v)

    # reverse finishing order
    for u in reversed(order):
        if comp[u] == -1:
            comp[u] = cid
            dfs2(u)
            cid += 1

    return comp, cid  # cid = number of components


def check(n, edges, extra_edges):
    """
    edges: list of (x, y, c) meaning A[x] >= A[y] + c
    extra_edges: same format, additional constraints
    Returns (ok, assignment list) where assignment is length n.
    """

    # Build graph for constraints: y -> x with weight c
    # SCC needs unweighted adjacency, so store endpoints only for SCC.
    adj = [[] for _ in range(n)]
    radj = [[] for _ in range(n)]

    def add_dir(u, v):
        adj[u].append(v)
        radj[v].append(u)

    for x, y, c in edges:
        add_dir(y, x)
    for x, y, c in extra_edges:
        add_dir(y, x)

    # Compute SCCs
    comp, k = kosaraju_scc(n, adj, radj)

    # Any positive edge inside SCC => impossible
    for x, y, c in edges:
        if comp[x] == comp[y] and c > 0:
            return False, []

    # Extra edges inside SCC are rejected (matches C++ behavior)
    for x, y, c in extra_edges:
        if comp[x] == comp[y]:
            return False, []

    # Build component DAG with weights
    comp_adj = [[] for _ in range(k)]  # comp_adj[u] = list of (v, w)
    indeg = [0] * k

    def add_comp_edge(cu, cv, w):
        comp_adj[cu].append((cv, w))
        indeg[cv] += 1

    for x, y, c in edges:
        cx, cy = comp[x], comp[y]
        if cx != cy:
            # cy -> cx, because y -> x
            add_comp_edge(cy, cx, c)

    for x, y, c in extra_edges:
        cx, cy = comp[x], comp[y]
        if cx != cy:
            add_comp_edge(cy, cx, c)

    # Forward topo DP: minimal feasible values starting from -10000
    comp_val = [-10000] * k

    q = deque([i for i in range(k) if indeg[i] == 0])
    topo = []

    indeg_work = indeg[:]  # copy because we'll mutate it
    while q:
        u = q.popleft()
        topo.append(u)
        for v, w in comp_adj[u]:
            # enforce val[v] >= val[u] + w
            nv = comp_val[u] + w
            if nv > comp_val[v]:
                comp_val[v] = nv
            indeg_work[v] -= 1
            if indeg_work[v] == 0:
                q.append(v)

    # Tighten pass (reverse topological): raise values to eliminate slack
    for u in reversed(topo):
        best = None
        for v, w in comp_adj[u]:
            # val[u] <= val[v] - w
            cand = comp_val[v] - w
            if best is None or cand < best:
                best = cand
        if best is not None:
            comp_val[u] = best

    # Expand back to vertices, check bounds
    ans = [0] * n
    for i in range(n):
        val = comp_val[comp[i]]
        if val < -10000 or val > 10000:
            return False, []
        ans[i] = val

    return True, ans


def solve():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    edges = []
    for _ in range(m):
        x = int(next(it)) - 1
        y = int(next(it)) - 1
        c = int(next(it))
        edges.append((x, y, c))

    # Base check
    ok, base_ans = check(n, edges, [])
    if not ok:
        sys.stdout.write("-1\n")
        return

    # Binary search K in [-20000, 20000] for constraint A[0] >= A[n-1] + K
    lo, hi = -20000, 20000
    best_ans = None

    while lo <= hi:
        mid = (lo + hi) // 2
        ok, res = check(n, edges, [(0, n - 1, mid)])
        if ok:
            best_ans = res
            lo = mid + 1
        else:
            hi = mid - 1

    if best_ans is None:
        best_ans = base_ans

    sys.stdout.write(" ".join(map(str, best_ans)) + "\n")


if __name__ == "__main__":
    solve()
