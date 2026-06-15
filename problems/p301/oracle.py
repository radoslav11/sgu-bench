import sys
import heapq

INF = 10**30

def solve() -> None:
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)

    n = next(it)
    x = next(it) - 1   # 0-based
    y = next(it) - 1

    m = next(it)

    # Build adjacency list for undirected weighted graph
    adj = [[] for _ in range(n)]
    for _ in range(m):
        u = next(it) - 1
        v = next(it) - 1
        w = next(it)
        adj[u].append((v, w))
        adj[v].append((u, w))

    # 1) Dijkstra from x to get shortest distances dist[]
    dist = [INF] * n
    dist[x] = 0
    pq = [(0, x)]  # (distance, node)

    while pq:
        d, u = heapq.heappop(pq)
        if d != dist[u]:
            continue  # stale entry
        for v, w in adj[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))

    # 2) Build reverse edges of shortest-path DAG:
    # if dist[u] + w == dist[v], then u -> v is in DAG, so store u in dag_rev[v]
    dag_rev = [[] for _ in range(n)]
    for u in range(n):
        du = dist[u]
        for v, w in adj[u]:
            if du + w == dist[v]:
                dag_rev[v].append(u)

    # 3) Mark all nodes that can reach y using only DAG edges (reverse DFS/BFS from y)
    reachable = [False] * n
    stack = [y]
    while stack:
        u = stack.pop()
        if reachable[u]:
            continue
        reachable[u] = True
        for p in dag_rev[u]:   # predecessors in DAG
            if not reachable[p]:
                stack.append(p)

    # 4) Build offline sweep-line events:
    # event = (pos, type, data), type: 0=IN, 1=QUERY, 2=OUT
    events = []

    for u in range(n):
        if reachable[u]:
            # vertex point at time dist[u] (as IN and OUT at same position)
            events.append((dist[u], 0, -1))
            events.append((dist[u], 2, -1))

        # for each DAG edge u->v with reachable[v], add range [dist[u]+1, dist[v]-1]
        du = dist[u]
        for v, w in adj[u]:
            if du + w == dist[v] and reachable[v]:
                l = du + 1
                r = dist[v] - 1
                if l <= r:
                    events.append((l, 0, -1))
                    events.append((r, 2, -1))

    # queries: answer[k] at time dist[k] for reachable nodes
    for k in range(n):
        if reachable[k]:
            events.append((dist[k], 1, k))

    # Sort so that at same pos: IN (0) before QUERY (1) before OUT (2)
    events.sort()

    ans = [0] * n
    balance = 0
    for pos, typ, dat in events:
        if typ == 0:
            balance += 1
        elif typ == 1:
            ans[dat] = balance
        else:
            balance -= 1

    # Print all answers space-separated (matching the C++ behavior)
    sys.stdout.write(" ".join(map(str, ans)) + "\n")


if __name__ == "__main__":
    solve()
