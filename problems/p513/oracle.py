import sys
sys.setrecursionlimit(10**7)


class DSU:
    def __init__(self, n):
        self.p = list(range(n))
        self.sz = [1] * n

    def find(self, x):
        if self.p[x] != x:
            self.p[x] = self.find(self.p[x])
        return self.p[x]

    def union(self, a, b):
        a = self.find(a); b = self.find(b)
        if a == b:
            return
        if self.sz[a] > self.sz[b]:
            a, b = b, a
        self.p[a] = b
        self.sz[b] += self.sz[a]

    def same(self, a, b):
        return self.find(a) == self.find(b)


def main():
    data = sys.stdin.read().split()
    n, m = map(int, data[:2])
    edges = data[2:]
    adj = [[False] * n for _ in range(n)]
    ptr = 0
    for _ in range(m):
        u = int(edges[ptr]) - 1
        v = int(edges[ptr + 1]) - 1
        ptr += 2
        adj[u][v] = adj[v][u] = True

    if n % 3 != 0:
        print("NO"); return

    triples_at = [[] for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if not adj[i][j]:
                for k in range(j + 1, n):
                    if not adj[i][k] and not adj[j][k]:
                        triple = (i, j, k)
                        triples_at[i].append(triple)
                        triples_at[j].append(triple)
                        triples_at[k].append(triple)

    for lst in triples_at:
        if len(lst) != 1:
            print("NO"); return

    for i in range(n):
        i2, j, k = triples_at[i][0]
        for a, b in [(i2, j), (i2, k), (j, k)]:
            adj[a][b] = adj[b][a] = True

    # mark diagonal as present so the non-edge degree below excludes self-loops
    for i in range(n):
        adj[i][i] = True

    dsu = DSU(2 * n)
    for u in range(n):
        for v in range(u + 1, n):
            if not adj[u][v]:
                dsu.union(u, v + n)
                dsu.union(u + n, v)

    for i in range(n):
        if dsu.same(i, i + n):
            print("NO"); return

    deg = [0] * n
    for i in range(n):
        deg[i] = adj[i].count(False)

    seen = [False] * (2 * n)
    for i in range(n):
        ri, rni = dsu.find(i), dsu.find(i + n)
        if seen[ri]:
            continue
        seen[ri] = seen[rni] = True

        cntA = cntB = sumDeg = 0
        for x in range(n):
            rx = dsu.find(x)
            if rx == ri:
                cntA += 1
                sumDeg += deg[x]
            elif rx == rni:
                cntB += 1
                sumDeg += deg[x]
        if sumDeg != 2 * cntA * cntB:
            print("NO"); return

    print("YES")


if __name__ == "__main__":
    main()
