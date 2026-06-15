import sys
from bisect import bisect_left


class DSU:
    def __init__(self, n: int):
        self.par = list(range(n))
        self.sz = [1] * n

    def root(self, x: int) -> int:
        while self.par[x] != x:
            self.par[x] = self.par[self.par[x]]
            x = self.par[x]
        return x

    def connected(self, a: int, b: int) -> bool:
        return self.root(a) == self.root(b)

    def unite(self, a: int, b: int) -> None:
        a = self.root(a)
        b = self.root(b)
        if a == b:
            return
        if self.sz[a] > self.sz[b]:
            a, b = b, a
        self.par[a] = b
        self.sz[b] += self.sz[a]


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    wires = []
    nodes = []

    idx = 1
    for _ in range(n):
        a = int(data[idx]); b = int(data[idx + 1])
        r = int(data[idx + 2]); p = int(data[idx + 3])
        idx += 4
        wires.append([a, b, r, p])
        nodes.append(a)
        nodes.append(b)

    nodes = sorted(set(nodes))
    for w in wires:
        w[0] = bisect_left(nodes, w[0])
        w[1] = bisect_left(nodes, w[1])

    m = len(nodes)
    dsu = DSU(m)

    perm = list(range(n))
    perm.sort(key=lambda i: (wires[i][2], wires[i][3]))

    total_cost = 0
    for k in range(n - 1, -1, -1):
        i = perm[k]
        u, v, r, p = wires[i]
        if not dsu.connected(u, v):
            dsu.unite(u, v)
            total_cost += p

    out = []
    out.append(str(total_cost))
    out.append(" ".join(str(i + 1) for i in perm))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
