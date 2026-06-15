"""Second oracle for p314: k shortest (non-simple) walk lengths s -> t.

Eppstein's algorithm, ported from the reference C++. Reverse Dijkstra
from t gives dist[] and a shortest-path arborescence; every non-tree
edge (u, v, w) is a "sidetrack" with penalty w + dist[v] - dist[u].
Each vertex owns a persistent meldable heap of the sidetracks usable
from it, built as own sidetracks melded into the parent's heap; the
global best-first search over (cost, heap node) then emits the k
smallest walk lengths.
"""

import heapq
import random
import sys


def main():
    sys.setrecursionlimit(300000)
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    k = int(next(it))
    s = int(next(it)) - 1
    t = int(next(it)) - 1

    adj = [[] for _ in range(n)]
    radj = [[] for _ in range(n)]
    for _ in range(m):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        w = int(next(it))
        adj[u].append((v, w))
        radj[v].append((u, w))

    INF = float("inf")
    dist = [INF] * n
    dist[t] = 0
    hq = [(0, t)]
    while hq:
        d, u = heapq.heappop(hq)
        if d > dist[u]:
            continue
        for v, w in radj[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(hq, (nd, v))

    if dist[s] == INF:
        sys.stdout.write("\n".join(["NO"] * k) + "\n")
        return

    rng = random.Random(123456789)

    # Persistent randomized meldable heap; node = [pen, vert, left, right].
    def meld(a, b):
        if a is None:
            return b
        if b is None:
            return a
        if a[0] > b[0]:
            a, b = b, a
        if rng.getrandbits(1):
            return [a[0], a[1], meld(a[2], b), a[3]]
        return [a[0], a[1], a[2], meld(a[3], b)]

    # parent in the arborescence towards t, and own sidetrack heaps
    parent = [-1] * n
    own = [None] * n
    for u in range(n):
        if dist[u] == INF:
            continue
        tree_used = False
        for v, w in adj[u]:
            if dist[v] == INF:
                continue
            if not tree_used and u != t and dist[u] == w + dist[v]:
                parent[u] = v
                tree_used = True
                continue
            own[u] = meld(own[u], [w + dist[v] - dist[u], v, None, None])

    # H[u] = own[u] melded with H[parent[u]]; build top-down from t
    H = [None] * n
    children = [[] for _ in range(n)]
    for u in range(n):
        if parent[u] != -1:
            children[parent[u]].append(u)
    order = [t]
    head = 0
    while head < len(order):
        u = order[head]
        head += 1
        H[u] = meld(own[u], H[parent[u]] if parent[u] != -1 else None)
        for c in children[u]:
            order.append(c)

    ans = [dist[s]]
    pq = []
    if H[s] is not None:
        heapq.heappush(pq, (dist[s] + H[s][0], id(H[s]), H[s]))
    while pq and len(ans) < k:
        d, _, node = heapq.heappop(pq)
        ans.append(d)
        pen, v, left, right = node
        if left is not None:
            heapq.heappush(pq, (d - pen + left[0], id(left), left))
        if right is not None:
            heapq.heappush(pq, (d - pen + right[0], id(right), right))
        if H[v] is not None:
            heapq.heappush(pq, (d + H[v][0], id(H[v]), H[v]))

    out = [str(x) for x in ans] + ["NO"] * (k - len(ans))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
