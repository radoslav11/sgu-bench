import sys

# -------------- Min-Cost Circulation via Negative-Cycle Cancelling --------------

class Edge:
    __slots__ = ("to", "rev", "cap", "cost")
    def __init__(self, to, rev, cap, cost):
        self.to = to        # endpoint
        self.rev = rev      # index of reverse edge in g[to]
        self.cap = cap      # residual capacity
        self.cost = cost    # cost per unit flow

class MinCostCirculation:
    """
    Finds a minimum-cost circulation in a directed graph with capacities and costs.
    Works with negative cycles by repeatedly canceling them (Bellman-Ford).
    Graph size here is small enough for this approach.
    """
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]

    def add_edge(self, fr, to, cap, cost):
        """
        Add edge fr->to and reverse edge to->fr.
        Returns (fr, index) to identify the forward edge later.
        """
        fwd = Edge(to, len(self.g[to]), cap, cost)
        rev = Edge(fr, len(self.g[fr]), 0, -cost)
        self.g[fr].append(fwd)
        self.g[to].append(rev)
        return fr, len(self.g[fr]) - 1

    def min_cost_circulation(self):
        """
        Cycle-canceling:
        - While there exists a negative cycle in the residual graph,
          send as much flow as possible around it.
        Returns total min cost.
        """
        n = self.n
        cost_total = 0

        while True:
            # Bellman-Ford to detect any negative cycle reachable from a super source.
            # We simulate super source by initializing dist=0 for all nodes.
            dist = [0] * n
            parent_v = [-1] * n      # parent vertex in relaxation
            parent_e = [-1] * n      # parent edge index in g[parent_v]

            x = -1
            for _ in range(n):
                x = -1
                for v in range(n):
                    dv = dist[v]
                    for ei, e in enumerate(self.g[v]):
                        if e.cap <= 0:
                            continue
                        nd = dv + e.cost
                        if nd < dist[e.to]:
                            dist[e.to] = nd
                            parent_v[e.to] = v
                            parent_e[e.to] = ei
                            x = e.to

            if x == -1:
                # No relaxation on nth iteration => no negative cycle
                break

            # 'x' is on or reaches a negative cycle; move x inside the cycle
            for _ in range(n):
                x = parent_v[x]

            # Reconstruct the cycle by walking parents until we return to x
            cycle = []
            cur = x
            while True:
                pv = parent_v[cur]
                pe = parent_e[cur]
                cycle.append((pv, pe))  # edge pv -> cur is pe in g[pv]
                cur = pv
                if cur == x:
                    break

            # Determine bottleneck capacity along the cycle
            add = 10**18
            for v, ei in cycle:
                e = self.g[v][ei]
                if e.cap < add:
                    add = e.cap

            # Augment flow along the cycle and update total cost
            for v, ei in cycle:
                e = self.g[v][ei]
                rev = self.g[e.to][e.rev]
                e.cap -= add
                rev.cap += add
                cost_total += add * e.cost

        return cost_total

# -------------- Problem Solution --------------

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m1 = int(next(it))
    m2 = int(next(it))

    # G[u][v] bitmask: 1 for Juan, 2 for Rosa
    G = [[0] * n for _ in range(n)]
    for _ in range(m1):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        G[u][v] |= 1
        G[v][u] |= 1
    for _ in range(m2):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        G[u][v] |= 2
        G[v][u] |= 2

    # Bipartite coloring using all edges that exist in either set
    color = [-1] * n
    sys.setrecursionlimit(10000)

    def dfs(u, c):
        color[u] = c
        for v in range(n):
            if G[u][v] == 0:
                continue
            if color[v] == -1:
                if not dfs(v, 1 - c):
                    return False
            elif color[v] == color[u]:
                return False
        return True

    for i in range(n):
        if color[i] == -1:
            ok = dfs(i, 0)
            # guaranteed bipartite by the original problem setting
            if not ok:
                raise RuntimeError("Graph is not bipartite")

    # Build circulation graph on 2*n nodes:
    # node u = "in", node u+n = "out"
    N = 2 * n
    mcc = MinCostCirculation(N)

    # For reconstruction, store (fr, idx, u, v, typ)
    edge_info = []

    for u in range(n):
        # capacity 1 through each vertex
        mcc.add_edge(u, u + n, 1, 0)

    for i in range(n):
        for j in range(i + 1, n):
            if G[i][j] == 0:
                continue

            # Juan edge: A(0) -> B(1)
            if G[i][j] & 1:
                if color[i] == 0 and color[j] == 1:
                    fr, idx = mcc.add_edge(i + n, j, 1, -1)
                    edge_info.append((fr, idx, i, j, 1))
                else:
                    fr, idx = mcc.add_edge(j + n, i, 1, -1)
                    edge_info.append((fr, idx, i, j, 1))

            # Rosa edge: B(1) -> A(0)
            if G[i][j] & 2:
                if color[i] == 1 and color[j] == 0:
                    fr, idx = mcc.add_edge(i + n, j, 1, -1)
                    edge_info.append((fr, idx, i, j, 2))
                else:
                    fr, idx = mcc.add_edge(j + n, i, 1, -1)
                    edge_info.append((fr, idx, i, j, 2))

    min_cost = mcc.min_cost_circulation()
    k = (-min_cost) // 2  # each marriage corresponds to 2 selected edges

    juan = []
    rosa = []

    # An original edge is chosen if its residual cap is 0 (since initial cap was 1)
    for fr, idx, u, v, typ in edge_info:
        e = mcc.g[fr][idx]
        used = (e.cap == 0)
        if used:
            if typ == 1:
                juan.append((u, v))
            else:
                rosa.append((u, v))

    out = []
    out.append(str(k))
    for u, v in juan:
        out.append(f"{u+1} {v+1}")
    for u, v in rosa:
        out.append(f"{u+1} {v+1}")
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
