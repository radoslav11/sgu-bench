import sys
from collections import deque

sys.setrecursionlimit(10**7)
input = sys.stdin.readline


class Dinic:
    """Dinic's algorithm for max-flow / min-cut."""

    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n + 1)]

    def add_edge(self, u, v, cap, idx=-1):
        """Add directed edge u->v with capacity cap and original index idx."""
        # forward edge: (to, capacity, idx, rev)
        self.adj[u].append([v, cap, idx, len(self.adj[v])])
        # backward edge: zero capacity
        self.adj[v].append([u, 0, -1, len(self.adj[u]) - 1])

    def bfs(self, s, t, level):
        """Build level graph via BFS."""
        for i in range(len(level)):
            level[i] = -1
        queue = deque([s])
        level[s] = 0
        while queue:
            u = queue.popleft()
            for v, cap, idx, rev in self.adj[u]:
                if cap > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    queue.append(v)
        return level[t] >= 0

    def dfs(self, u, t, flow, level, it):
        """Push flow along DFS in level graph."""
        if u == t:
            return flow
        for i in range(it[u], len(self.adj[u])):
            it[u] = i
            v, cap, idx, rev = self.adj[u][i]
            if cap > 0 and level[v] == level[u] + 1:
                pushed = self.dfs(v, t, min(flow, cap), level, it)
                if pushed:
                    # update residual capacities
                    self.adj[u][i][1] -= pushed
                    self.adj[v][rev][1] += pushed
                    return pushed
        return 0

    def max_flow(self, s, t):
        """Compute max flow s->t."""
        flow = 0
        level = [-1] * (self.n + 1)
        while self.bfs(s, t, level):
            it = [0] * (self.n + 1)
            pushed = self.dfs(s, t, 10**18, level, it)
            while pushed:
                flow += pushed
                pushed = self.dfs(s, t, 10**18, level, it)
        return flow

    def min_cut_edges(self, s):
        """After max_flow, find reachable from s in residual graph."""
        vis = [False] * (self.n + 1)
        stack = [s]
        vis[s] = True
        while stack:
            u = stack.pop()
            for v, cap, idx, rev in self.adj[u]:
                # if residual cap > 0, can go
                if cap > 0 and not vis[v]:
                    vis[v] = True
                    stack.append(v)
        return vis


def main():
    n, m = map(int, input().split())
    edges = [None] * (m + 1)
    cost = [0] * (m + 1)
    adj = [[] for _ in range(n + 1)]
    for i in range(1, m + 1):
        a, b, w = map(int, input().split())
        edges[i] = (a, b)
        cost[i] = w
        adj[a].append(b)
        adj[b].append(a)

    best = None
    best_list = []

    # Try capturing each city r. A cut that splits some pair of r's
    # neighbours also splits nbrs[0] from one of them, so it is enough to
    # run max-flow from nbrs[0] to each other neighbour instead of over
    # all pairs.
    for r in range(1, n + 1):
        nbrs = sorted(set(adj[r]))
        for v in nbrs[1:]:
            u = nbrs[0]
            # Build flow network on nodes 1..n excluding r
            din = Dinic(n)
            for idx in range(1, m + 1):
                a, b = edges[idx]
                if a == r or b == r:
                    continue
                # Undirected -> two directed edges
                din.add_edge(a, b, cost[idx], idx)
                din.add_edge(b, a, cost[idx], idx)
            # Compute min-cut between u and v
            f = din.max_flow(u, v)
            if best is None or f < best:
                # Identify which original edges are in the cut
                reachable = din.min_cut_edges(u)
                cut_edges = set()
                for x in range(1, n + 1):
                    if not reachable[x]:
                        continue
                    for to, cap, idx, rev in din.adj[x]:
                        if not reachable[to] and idx != -1:
                            cut_edges.add(idx)
                cut_list = sorted(cut_edges)
                best = f
                best_list = cut_list

    # Output
    print(best)
    print(len(best_list))
    if best_list:
        print(*best_list)


if __name__ == "__main__":
    main()
