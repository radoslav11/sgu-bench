import sys
from collections import deque

class Dinic:
    """Dinic's max flow for integer capacities."""
    class Edge:
        __slots__ = ('to','rev','cap','flow')
        def __init__(self, to, rev, cap):
            self.to = to         # target node
            self.rev = rev       # index of reverse edge in adj[to]
            self.cap = cap       # capacity
            self.flow = 0        # current flow

    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]
        self.level = [0]*n
        self.ptr = [0]*n

    def add_edge(self, u, v, cap):
        """Add directed edge u->v with capacity cap."""
        self.adj[u].append(Dinic.Edge(v, len(self.adj[v]), cap))
        # reverse edge has 0 capacity initially
        self.adj[v].append(Dinic.Edge(u, len(self.adj[u])-1, 0))

    def bfs(self, s, t):
        """Build level graph with BFS. Returns True if t is reachable."""
        for i in range(self.n):
            self.level[i] = -1
        self.level[s] = 0
        dq = deque([s])
        while dq:
            u = dq.popleft()
            for e in self.adj[u]:
                if self.level[e.to] < 0 and e.flow < e.cap:
                    self.level[e.to] = self.level[u] + 1
                    dq.append(e.to)
        return self.level[t] >= 0

    def dfs(self, u, t, pushed):
        """DFS to send flow on level graph."""
        if pushed == 0 or u == t:
            return pushed
        for i in range(self.ptr[u], len(self.adj[u])):
            e = self.adj[u][i]
            if self.level[e.to] == self.level[u] + 1 and e.flow < e.cap:
                tr = self.dfs(e.to, t, min(pushed, e.cap - e.flow))
                if tr > 0:
                    e.flow += tr
                    self.adj[e.to][e.rev].flow -= tr
                    return tr
            self.ptr[u] += 1
        return 0

    def max_flow(self, s, t):
        """Compute maximum flow from s to t."""
        flow = 0
        INF = 10**18
        while self.bfs(s, t):
            self.ptr = [0]*self.n
            while True:
                pushed = self.dfs(s, t, INF)
                if pushed == 0:
                    break
                flow += pushed
        return flow

def main():
    input = sys.stdin.readline
    n, m = map(int, input().split())
    edges = []
    # balance[i] = net demand after subtracting lower bounds
    balance = [0]*n
    for _ in range(m):
        u,v,l,c = map(int, input().split())
        u -= 1
        v -= 1
        edges.append((u, v, l, c))
        balance[u] -= l
        balance[v] += l

    # build flow network with n + 2 nodes (super-source, super-sink)
    S = n
    T = n+1
    dinic = Dinic(n+2)

    # add reduced-capacity edges
    for i,(u,v,l,c) in enumerate(edges):
        dinic.add_edge(u, v, c - l)

    # connect super-source and super-sink
    need = 0
    for i in range(n):
        if balance[i] > 0:
            # node i needs to push out balance[i]
            dinic.add_edge(S, i, balance[i])
            need += balance[i]
        elif balance[i] < 0:
            # node i must absorb -balance[i]
            dinic.add_edge(i, T, -balance[i])

    # compute max flow
    flow = dinic.max_flow(S, T)
    if flow != need:
        print("NO")
        return

    # recover actual flows
    print("YES")
    # we must iterate edges in the same order
    # find each u->v edge in adjacency to recover flow
    ptr = [0]*n  # pointer per node
    for u,v,l,c in edges:
        # find the edge u->v
        while True:
            e = dinic.adj[u][ptr[u]]
            ptr[u] += 1
            if e.to == v and e.cap + l == c:  # matches the reduced cap
                flow_uv = e.flow  # this is f' = f - l
                print(flow_uv + l)
                break

if __name__ == "__main__":
    main()
