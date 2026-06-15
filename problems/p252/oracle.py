import sys
import heapq

# Min-cost max-flow with potentials + Dijkstra (successive shortest augmenting path)
class MinCostFlow:
    def __init__(self, n=0):
        self.init(n)

    def init(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]   # adjacency list of edges
        self.potential = [0] * n          # Johnson potentials
        self.parent_v = [-1] * n          # parent vertex in shortest path tree
        self.parent_e = [-1] * n          # parent edge index in g[parent_v]
        self.INF = 10**18

    # Edge stored as [to, rev, cap, cost]
    # rev is the index of the reverse edge in g[to]
    def add_edge(self, fr, to, cap, cost):
        fwd = [to, len(self.g[to]), cap, cost]
        rev = [fr, len(self.g[fr]), 0, -cost]
        self.g[fr].append(fwd)
        self.g[to].append(rev)
        return len(self.g[fr]) - 1  # index of forward edge

    def flow(self, s, t, flow_limit=None):
        if flow_limit is None:
            flow_limit = self.INF

        n = self.n
        # Initialize potentials using Bellman-Ford-like relaxation.
        # Here costs are non-negative, so all zeros would work too,
        # but we keep it generic.
        pot = [self.INF] * n
        pot[s] = 0
        changed = True
        while changed:
            changed = False
            for v in range(n):
                if pot[v] == self.INF:
                    continue
                for to, rev, cap, cost in self.g[v]:
                    if cap > 0 and pot[v] + cost < pot[to]:
                        pot[to] = pot[v] + cost
                        changed = True
        # Replace INF with 0 for unreachable to avoid overflow in reduced costs;
        # they won't be used anyway because they remain unreachable in Dijkstra.
        self.potential = [0 if x == self.INF else x for x in pot]

        total_flow = 0
        total_cost = 0

        while total_flow < flow_limit:
            # Dijkstra on reduced costs
            dist = [self.INF] * n
            dist[s] = 0
            self.parent_v = [-1] * n
            self.parent_e = [-1] * n

            pq = [(0, s)]
            while pq:
                d, v = heapq.heappop(pq)
                if d != dist[v]:
                    continue
                for ei, e in enumerate(self.g[v]):
                    to, rev, cap, cost = e
                    if cap <= 0:
                        continue
                    # reduced cost
                    nd = d + cost + self.potential[v] - self.potential[to]
                    if nd < dist[to]:
                        dist[to] = nd
                        self.parent_v[to] = v
                        self.parent_e[to] = ei
                        heapq.heappush(pq, (nd, to))

            if dist[t] == self.INF:
                break  # no augmenting path

            # Update potentials for nodes reached by Dijkstra
            for i in range(n):
                if dist[i] < self.INF:
                    self.potential[i] += dist[i]

            # Find bottleneck capacity on the path
            add = flow_limit - total_flow
            v = t
            while v != s:
                pv = self.parent_v[v]
                pe = self.parent_e[v]
                if pv == -1:
                    add = 0
                    break
                add = min(add, self.g[pv][pe][2])  # cap
                v = pv
            if add == 0:
                break

            # Augment flow and compute real cost (use original costs)
            v = t
            while v != s:
                pv = self.parent_v[v]
                pe = self.parent_e[v]
                e = self.g[pv][pe]
                to, rev, cap, cost = e

                # send 'add' through forward edge
                e[2] -= add  # reduce residual capacity
                # increase residual capacity on reverse edge
                self.g[v][rev][2] += add

                total_cost += cost * add
                v = pv

            total_flow += add

        return total_cost, total_flow


def solve():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    edges = []
    for _ in range(m):
        a = int(next(it)); b = int(next(it)); c = int(next(it))
        edges.append((a, b, c))

    # Build flow network for bipartite min-cost max matching
    source = 0
    sink = 2 * n + 1
    mcf = MinCostFlow(2 * n + 2)

    # source -> left nodes, right nodes -> sink
    for i in range(1, n + 1):
        mcf.add_edge(source, i, 1, 0)
        mcf.add_edge(n + i, sink, 1, 0)

    # Add edges for each railroad a->b with cost c
    edge_ids = []  # (a, idx_in_g[a]) for later inspection via residuals
    for a, b, c in edges:
        idx = mcf.add_edge(a, n + b, 1, c)
        edge_ids.append((a, idx))

    # Run min-cost max-flow => min-cost maximum matching
    cost, matching = mcf.flow(source, sink)

    num_paths = n - matching

    # Reconstruct chosen matching:
    # If edge a->(n+b) was used, then its capacity became 0 (from 1),
    # because we pushed 1 unit through it.
    nxt = [0] * (n + 1)
    has_prev = [False] * (n + 1)

    for (a, b, c), (aa, idx) in zip(edges, edge_ids):
        e = mcf.g[aa][idx]
        # e[2] is residual capacity; if it is 0, the edge is saturated => chosen
        if e[2] == 0:
            nxt[a] = b
            has_prev[b] = True

    out = []
    out.append(f"{num_paths} {cost}")

    # Output each path starting from nodes with no predecessor
    for i in range(1, n + 1):
        if has_prev[i]:
            continue
        path = []
        v = i
        while v != 0:
            path.append(v)
            v = nxt[v]
        out.append(str(len(path)) + " " + " ".join(map(str, path)))

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
