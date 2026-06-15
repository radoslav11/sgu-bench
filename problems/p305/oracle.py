import sys
from collections import deque

# Hopcroft–Karp maximum bipartite matching:
# Left side: 0..n-1, Right side: 0..m-1
class HopcroftKarp:
    def __init__(self, n, m):
        self.n = n
        self.m = m
        self.adj = [[] for _ in range(n)]  # edges from left to right
        self.match = [-1] * m              # match[v] = u
        self.inv_match = [-1] * n          # inv_match[u] = v
        self.dist = [-1] * n               # BFS layers for left nodes

    def add_edge(self, u, v):
        self.adj[u].append(v)

    def bfs(self):
        q = deque()
        self.dist = [-1] * self.n

        # Start BFS from all free left nodes
        for u in range(self.n):
            if self.inv_match[u] == -1:
                self.dist[u] = 0
                q.append(u)

        found_free_right = False

        # BFS over alternating edges:
        # left u -> right v (unmatched edge), right v -> left match[v] (matched edge)
        while q:
            u = q.popleft()
            for v in self.adj[u]:
                mu = self.match[v]  # left node matched with v (or -1)
                if mu == -1:
                    found_free_right = True
                else:
                    if self.dist[mu] == -1:
                        self.dist[mu] = self.dist[u] + 1
                        q.append(mu)

        return found_free_right

    def dfs(self, u):
        for v in self.adj[u]:
            mu = self.match[v]
            # Either v is free, or we can recurse to rematch mu in next layer
            if mu == -1 or (self.dist[mu] == self.dist[u] + 1 and self.dfs(mu)):
                self.inv_match[u] = v
                self.match[v] = u
                return True

        # Mark u as dead end for this phase
        self.dist[u] = -1
        return False

    def max_matching(self):
        res = 0
        while self.bfs():
            for u in range(self.n):
                if self.inv_match[u] == -1 and self.dfs(u):
                    res += 1
        return res


def solve():
    it = iter(sys.stdin.read().strip().split())
    n = int(next(it))
    M = int(next(it))

    prog = []
    for _ in range(n):
        a = int(next(it)); b = int(next(it))
        prog.append((a, b))

    # Build up to n candidate values per progression that lie in [1..M]
    candidates = [[] for _ in range(n)]

    for i, (a, b) in enumerate(prog):
        if a == 0:
            # Constant progression: x = b
            if 1 <= b <= M:
                candidates[i].append(b)
        else:
            start_k = 0

            if a > 0:
                # Need smallest k >= 0 with a*k + b >= 1
                if b < 1:
                    # ceil((1-b)/a)
                    start_k = (1 - b + a - 1) // a
            else:
                # a < 0 decreasing: need a*k + b <= M
                if b > M:
                    # ceil((b-M)/(-a))
                    start_k = (b - M + (-a) - 1) // (-a)

            # Generate first up to n values from start_k, stop if out of bounds
            for j in range(n):
                x = a * (start_k + j) + b
                if x < 1 or x > M:
                    break
                candidates[i].append(x)

    # Coordinate compress all candidate values to a small right side
    all_vals = []
    for i in range(n):
        all_vals.extend(candidates[i])

    all_vals = sorted(set(all_vals))  # unique sorted
    idx = {v: k for k, v in enumerate(all_vals)}  # value -> compressed id

    # Build matching graph
    hk = HopcroftKarp(n, len(all_vals))
    for i in range(n):
        for v in candidates[i]:
            hk.add_edge(i, idx[v])

    hk.max_matching()

    # Construct final assignment
    ans = [-1] * n
    used = set()

    # Matched ones get their progression member
    for i in range(n):
        j = hk.inv_match[i]
        if j != -1:
            val = all_vals[j]
            ans[i] = val
            used.add(val)

    # Unmatched ones get smallest unused numbers in [1..M]
    nxt = 1
    for i in range(n):
        if ans[i] == -1:
            while nxt in used:
                nxt += 1
            ans[i] = nxt
            used.add(nxt)
            nxt += 1

    print(" ".join(map(str, ans)))


if __name__ == "__main__":
    solve()
