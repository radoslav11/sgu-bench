import sys
from collections import deque

class Dinic:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]
        self.level = [0]*n
        self.it = [0]*n

    def add_edge(self, u, v, c):
        # forward edge index len(adj[u]), backward edge index len(adj[v])
        self.adj[u].append([v, c, len(self.adj[v])])
        self.adj[v].append([u, 0, len(self.adj[u]) - 1])

    def bfs(self, S, T):
        for i in range(self.n):
            self.level[i] = -1
        dq = deque([S])
        self.level[S] = 0
        while dq:
            u = dq.popleft()
            for v, cap, rev in self.adj[u]:
                if cap > 0 and self.level[v] < 0:
                    self.level[v] = self.level[u] + 1
                    dq.append(v)
        return self.level[T] >= 0

    def dfs(self, u, T, flow):
        if u == T:
            return flow
        for i in range(self.it[u], len(self.adj[u])):
            self.it[u] = i
            v, cap, rev = self.adj[u][i]
            if cap and self.level[v] == self.level[u] + 1:
                pushed = self.dfs(v, T, min(flow, cap))
                if pushed:
                    # reduce forward, increase backward
                    self.adj[u][i][1] -= pushed
                    self.adj[v][rev][1] += pushed
                    return pushed
        return 0

    def maxflow(self, S, T):
        flow = 0
        INF = 10**18
        while self.bfs(S, T):
            self.it = [0]*self.n
            while True:
                pushed = self.dfs(S, T, INF)
                if not pushed:
                    break
                flow += pushed
        return flow

def solve():
    data = sys.stdin.read().split()
    it = iter(data)
    N = int(next(it))
    M = int(next(it))
    D = int(next(it))
    W = int(next(it))
    piles = []
    for _ in range(N):
        x = int(next(it)); y = int(next(it)); c = int(next(it))
        piles.append((x, y, c))

    # Special case: direct jump bank->bank
    if D >= W:
        print(1)
        return

    # Precompute adjacency by distance
    adj = [[False]*N for _ in range(N)]
    for i in range(N):
        x1,y1,_ = piles[i]
        for j in range(N):
            x2,y2,_ = piles[j]
            dx, dy = x1-x2, y1-y2
            adj[i][j] = (dx*dx + dy*dy <= D*D)

    # binary search minimal T such that flow >= M
    lo, hi = 0, N + M
    res = -1
    while lo <= hi:
        mid = (lo + hi)//2
        # build time-expanded graph for t=0..mid
        layers = mid + 1
        # nodes: source=0, sink=1, then for each (t,i), two nodes
        node_cnt = 2 + layers*N*2
        SRC, SNK = 0, 1
        dinic = Dinic(node_cnt)

        def in_id(t, i):
            return 2 + (t*N + i)*2
        def out_id(t, i):
            return 2 + (t*N + i)*2 + 1

        # add edges
        for t in range(layers):
            for i, (x,y,c) in enumerate(piles):
                # capacity of pile
                dinic.add_edge(in_id(t,i), out_id(t,i), c)
                # reachable from start bank
                if y <= D:
                    dinic.add_edge(SRC, in_id(t,i), 10**18)
                # can jump to far bank
                if y + D >= W:
                    dinic.add_edge(out_id(t,i), SNK, 10**18)
                # transitions from previous time
                if t > 0:
                    for j in range(N):
                        if adj[j][i]:  # pile j(t-1,out) -> i(t,in)
                            dinic.add_edge(out_id(t-1,j), in_id(t,i), 10**18)

        # compute max flow
        f = dinic.maxflow(SRC, SNK)
        if f >= M:
            res = mid
            hi = mid - 1
        else:
            lo = mid + 1

    if res < 0:
        print("IMPOSSIBLE")
    else:
        print(res + 2)

if __name__ == "__main__":
    solve()
