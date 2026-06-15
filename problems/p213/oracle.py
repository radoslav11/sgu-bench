import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    s = int(next(it)) - 1
    t = int(next(it)) - 1

    # Build adjacency list: adj[u] = list of (v, edge_id)
    adj = [[] for _ in range(n)]
    for eid in range(m):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        adj[u].append((v, eid))
        adj[v].append((u, eid))

    # BFS from s to compute distances
    dist = [-1]*n
    dist[s] = 0
    q = deque([s])
    while q:
        u = q.popleft()
        for v, _ in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)

    # D = shortest distance from s to t
    D = dist[t]

    # Prepare D lists of edge IDs
    answer = [[] for _ in range(D)]

    # For every node u with dist[u] < D
    for u in range(n):
        du = dist[u]
        if du < 0 or du >= D:
            continue
        # Check each outgoing edge
        for v, eid in adj[u]:
            # select only edges going one layer forward
            if dist[v] == du + 1:
                answer[du].append(eid + 1)  # 1-based

    # Print result
    out = []
    out.append(str(D))
    for level in range(D):
        lst = answer[level]
        out.append(f"{len(lst)} {' '.join(map(str, lst))}")
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
