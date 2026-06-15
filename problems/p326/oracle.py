import sys
from collections import deque

class Dinic:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]

    def add_edge(self, u, v, cap):
        # forward edge index = len(adj[u])
        # backward edge index = len(adj[v])
        self.adj[u].append([v, cap, len(self.adj[v])])
        self.adj[v].append([u, 0,   len(self.adj[u]) - 1])

    def bfs(self, s, t, level):
        for i in range(self.n):
            level[i] = -1
        queue = deque([s])
        level[s] = 0
        while queue:
            u = queue.popleft()
            for v, cap, rev in self.adj[u]:
                if cap > 0 and level[v] < 0:
                    level[v] = level[u] + 1
                    queue.append(v)
        return level[t] >= 0

    def dfs(self, u, t, f, level, it):
        if u == t:
            return f
        for i in range(it[u], len(self.adj[u])):
            v, cap, rev = self.adj[u][i]
            if cap > 0 and level[v] == level[u] + 1:
                pushed = self.dfs(v, t, min(f, cap), level, it)
                if pushed:
                    # reduce forward capacity
                    self.adj[u][i][1] -= pushed
                    # increase backward capacity
                    self.adj[v][rev][1] += pushed
                    return pushed
            it[u] += 1
        return 0

    def max_flow(self, s, t):
        flow = 0
        level = [-1]*self.n
        while self.bfs(s, t, level):
            it = [0]*self.n
            while True:
                pushed = self.dfs(s, t, 10**18, level, it)
                if pushed == 0:
                    break
                flow += pushed
        return flow

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    N = int(next(it))
    w = [int(next(it)) for _ in range(N)]
    r = [int(next(it)) for _ in range(N)]
    cnt = [ [int(next(it)) for _ in range(N)] for _ in range(N) ]

    # Max wins for team 1
    W1 = w[0] + r[0]
    # Immediate elimination check
    for i in range(1, N):
        if w[i] > W1:
            print("NO")
            return

    # Collect games among opponents 2..N
    games = []
    total_games = 0
    for i in range(1, N):
        for j in range(i+1, N):
            g = cnt[i][j]
            if g > 0:
                games.append((i, j, g))
                total_games += g

    M = len(games)
    S = N + M
    T = N + M + 1
    dinic = Dinic(N + M + 2)

    # Build graph edges
    # source -> game nodes
    for idx, (i, j, g) in enumerate(games):
        game_node = N + idx
        dinic.add_edge(S, game_node, g)
        dinic.add_edge(game_node, i, g)
        dinic.add_edge(game_node, j, g)

    # team nodes -> sink
    for i in range(1, N):
        cap = W1 - w[i]
        dinic.add_edge(i, T, cap)

    # compute max flow
    flow = dinic.max_flow(S, T)
    print("YES" if flow == total_games else "NO")

if __name__ == "__main__":
    main()
