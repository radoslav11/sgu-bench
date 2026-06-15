import sys
import heapq
import threading
from collections import Counter

sys.setrecursionlimit(1 << 21)


class LCA_RMQ:
    """LCA with Euler tour + sparse table RMQ on (depth, node)."""

    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]
        self.euler = []
        self.first = [-1] * n
        self.depth = [0] * n
        self.log = []
        self.st = []

    def add_edge(self, u, v):
        self.adj[u].append(v)
        self.adj[v].append(u)

    def _dfs(self, u, p, d):
        self.first[u] = len(self.euler)
        self.depth[u] = d
        self.euler.append((d, u))
        for v in self.adj[u]:
            if v == p:
                continue
            self._dfs(v, u, d + 1)
            self.euler.append((d, u))

    def prepare(self, root=0):
        self.euler = []
        self._dfs(root, -1, 0)
        m = len(self.euler)

        self.log = [0] * (m + 1)
        for i in range(2, m + 1):
            self.log[i] = self.log[i // 2] + 1

        self.st = [self.euler[:]]
        j = 1
        while (1 << j) <= m:
            prev = self.st[j - 1]
            curr = []
            span = 1 << j
            half = span >> 1
            for i in range(0, m - span + 1):
                curr.append(prev[i] if prev[i] < prev[i + half] else prev[i + half])
            self.st.append(curr)
            j += 1

    def lca(self, u, v):
        fu, fv = self.first[u], self.first[v]
        if fu > fv:
            fu, fv = fv, fu
        length = fv - fu + 1
        k = self.log[length]
        left = self.st[k][fu]
        right = self.st[k][fv - (1 << k) + 1]
        return left[1] if left < right else right[1]


class MultiSetMin:
    """Multiset as Counter + min-heap with lazy deletion."""

    __slots__ = ("cnt", "heap", "size")

    def __init__(self):
        self.cnt = Counter()
        self.heap = []
        self.size = 0

    def add(self, x):
        self.cnt[x] += 1
        heapq.heappush(self.heap, x)
        self.size += 1

    def discard_one(self, x):
        if self.cnt[x] > 0:
            self.cnt[x] -= 1
            self.size -= 1

    def get_min(self):
        while self.heap and self.cnt[self.heap[0]] == 0:
            heapq.heappop(self.heap)
        return self.heap[0] if self.heap else None

    def merge_from(self, other):
        for x, c in other.cnt.items():
            if c:
                self.cnt[x] += c
                self.size += c
                heapq.heappush(self.heap, x)


def solve():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    tree = [[] for _ in range(n)]
    nontree = [[] for _ in range(n)]

    lca = LCA_RMQ(n)

    for _ in range(m):
        a = int(next(it)) - 1
        b = int(next(it)) - 1
        w = int(next(it))
        t = int(next(it))
        if t == 1:
            tree[a].append((b, w))
            tree[b].append((a, w))
            lca.add_edge(a, b)
        else:
            nontree[a].append((b, w))
            nontree[b].append((a, w))

    parent = [-1] * n
    dist = [0] * n

    stack = [(0, -1)]
    while stack:
        u, p = stack.pop()
        parent[u] = p
        for v, w in tree[u]:
            if v == p:
                continue
            dist[v] = dist[u] + w
            stack.append((v, u))

    lca.prepare(0)

    addv = [[] for _ in range(n)]
    remv = [[] for _ in range(n)]

    for u in range(n):
        for v, w in nontree[u]:
            if u < v:
                val = dist[u] + dist[v] + w
                c = lca.lca(u, v)
                addv[u].append(val)
                addv[v].append(val)
                remv[c].append(val)
                remv[c].append(val)

    ans = [-1] * n

    def dfs(u):
        cur = MultiSetMin()

        for v, _w in tree[u]:
            if v == parent[u]:
                continue
            child = dfs(v)
            if child.size > cur.size:
                cur, child = child, cur
            cur.merge_from(child)

        for x in addv[u]:
            cur.add(x)

        for x in remv[u]:
            cur.discard_one(x)

        if u != 0:
            mn = cur.get_min()
            ans[u] = -1 if mn is None else (mn - dist[u])

        return cur

    dfs(0)

    out = " ".join(str(ans[i]) for i in range(1, n))
    sys.stdout.write(out + "\n")


if __name__ == "__main__":
    threading.stack_size(1 << 28)
    t = threading.Thread(target=solve)
    t.start()
    t.join()
