import sys

sys.setrecursionlimit(10**7)


class Dinic:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]

    def add_edge(self, u, v, c, idx=-1):
        self.adj[u].append([v, c, 0, len(self.adj[v]), idx])
        self.adj[v].append([u, 0, 0, len(self.adj[u]) - 1, -1])

    def bfs(self, s, t, level):
        for i in range(self.n):
            level[i] = -1
        queue = [s]
        level[s] = 0
        for u in queue:
            for v, cap, flow, rev, _ in self.adj[u]:
                if level[v] < 0 and flow < cap:
                    level[v] = level[u] + 1
                    queue.append(v)
        return level[t] >= 0

    def dfs(self, u, t, f, level, ptr):
        if u == t:
            return f
        while ptr[u] < len(self.adj[u]):
            i = ptr[u]
            v, cap, flow, rev, _ = self.adj[u][i]
            if level[v] == level[u] + 1 and flow < cap:
                pushed = self.dfs(v, t, min(f, cap - flow), level, ptr)
                if pushed:
                    self.adj[u][i][2] += pushed
                    self.adj[v][rev][2] -= pushed
                    return pushed
            ptr[u] += 1
        return 0

    def max_flow(self, s, t):
        flow = 0
        level = [-1] * self.n
        while self.bfs(s, t, level):
            ptr = [0] * self.n
            while True:
                pushed = self.dfs(s, t, 10**18, level, ptr)
                if not pushed:
                    break
                flow += pushed
        return flow


def read_input():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    edges = []
    for _ in range(m):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        cap = int(next(it))
        flag = int(next(it))
        lb = cap if flag == 1 else 0
        edges.append((u, v, cap, lb))
    return n, m, edges


def build_graph(n, edges, x):
    S, T = n, n + 1
    mf = Dinic(n + 2)
    demand = [0] * n
    for idx, (u, v, cap, lb) in enumerate(edges):
        demand[v] += lb
        demand[u] -= lb
        mf.add_edge(u, v, cap - lb, idx)
    for i in range(n):
        if demand[i] > 0:
            mf.add_edge(S, i, demand[i])
        elif demand[i] < 0:
            mf.add_edge(i, T, -demand[i])
    mf.add_edge(n - 1, 0, x)
    return mf, demand


def feasible(n, edges, x):
    mf, demand = build_graph(n, edges, x)
    need = sum(d for d in demand if d > 0)
    got = mf.max_flow(n, n + 1)
    return got == need


def main():
    n, m, edges = read_input()

    lo, hi = 0, 10**8
    ans = -1
    while lo <= hi:
        mid = (lo + hi) // 2
        if feasible(n, edges, mid):
            ans = mid
            hi = mid - 1
        else:
            lo = mid + 1

    if ans < 0:
        print("Impossible")
        return

    mf, demand = build_graph(n, edges, ans)
    mf.max_flow(n, n + 1)
    res = [0] * m
    for u in range(n):
        for v, cap, flow, rev, idx in mf.adj[u]:
            if idx >= 0:
                lb = edges[idx][3]
                res[idx] = flow + lb

    print(ans)
    print(*res)


if __name__ == "__main__":
    main()
