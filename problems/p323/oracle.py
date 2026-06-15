import sys
sys.setrecursionlimit(1_000_000)

class DSU:
    """Disjoint Set Union with path compression + union by size."""
    def __init__(self, n: int):
        # We'll use elements 0..n-1
        self.parent = list(range(n))
        self.size = [1] * n

    def find(self, x: int) -> int:
        # Path compression
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, a: int, b: int) -> bool:
        # Returns True if merged, False if already in same set
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        # Union by size: attach smaller tree to larger tree
        if self.size[ra] > self.size[rb]:
            ra, rb = rb, ra
        self.parent[ra] = rb
        self.size[rb] += self.size[ra]
        return True

    def connected(self, a: int, b: int) -> bool:
        return self.find(a) == self.find(b)


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    n = int(next(it))
    m = int(next(it))
    k = int(next(it))

    # edges: (p, c, u, v, idx)
    edges = []
    # airlines[c] = list of (u, v) owned by airline c
    airlines = [[] for _ in range(m)]

    for idx in range(k):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        c = int(next(it)) - 1
        p = int(next(it))
        edges.append((p, c, u, v, idx))
        airlines[c].append((u, v))

    # 1) Global MST by p => relevant edges
    edges.sort()  # sorts by p primarily
    dsu_global = DSU(n)
    relevant = []
    for p, c, u, v, idx in edges:
        if dsu_global.union(u, v):
            relevant.append((p, c, u, v, idx))
            # At most n-1 edges will be collected

    # 2) Try each airline as the one to keep
    best_cost = 10**30
    best_airline = -1
    best_purchases = []

    for keep in range(m):
        dsu = DSU(n)

        # Add all edges of airline 'keep' for free
        for u, v in airlines[keep]:
            dsu.union(u, v)

        cost = 0
        purchases = []

        # Add only relevant MST edges (Kruskal-style)
        for p, c, u, v, idx in relevant:
            if not dsu.connected(u, v):
                dsu.union(u, v)
                cost += p
                purchases.append(idx + 1)  # output must be 1-based

        if cost < best_cost:
            best_cost = cost
            best_airline = keep
            best_purchases = purchases

    # Output
    out = []
    out.append(f"{best_cost} {best_airline + 1} {len(best_purchases)}\n")
    for idx in best_purchases:
        out.append(f"{idx}\n")
    sys.stdout.write("".join(out))


if __name__ == "__main__":
    solve()
