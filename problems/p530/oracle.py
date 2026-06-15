import sys
from collections import deque

INF = 10**18

def bfs(start, adj, n):
    """Return array d where d[v] is shortest distance from start, or INF if unreachable."""
    d = [-1] * n
    q = deque([start])
    d[start] = 0

    while q:
        u = q.popleft()
        for v in adj[u]:
            if d[v] == -1:
                d[v] = d[u] + 1
                q.append(v)

    # Convert -1 to INF for easier comparisons later
    return [INF if x == -1 else x for x in d]


def solve():
    input = sys.stdin.readline
    n, m = map(int, input().split())

    # Build undirected graph
    adj = [[] for _ in range(n)]
    for _ in range(m):
        u, v = map(int, input().split())
        u -= 1
        v -= 1
        adj[u].append(v)
        adj[v].append(u)

    # distances[v][k] = distance from genius k (0,1,2) to v
    d0 = bfs(0, adj, n)
    d1 = bfs(1, adj, n)
    d2 = bfs(2, adj, n)
    distances = [d0, d1, d2]  # distances[k][v]
    # For convenience in comparator-like access, we’ll also expose dist(v,k)
    # as dist_vk[v][k]
    dist_vk = list(zip(d0, d1, d2))  # tuple per vertex: (to g0, to g1, to g2)

    nodes = list(range(n))
    is_dominated = [False] * n

    perms = [
        (0, 1, 2),
        (0, 2, 1),
        (1, 0, 2),
        (1, 2, 0),
        (2, 0, 1),
        (2, 1, 0),
    ]

    for primary, secondary, tertiary in perms:
        # Sort by (dist to primary, then to secondary, then to tertiary, then id)
        nodes.sort(key=lambda v: (dist_vk[v][primary], dist_vk[v][secondary], dist_vk[v][tertiary], v))

        # seen pairs from strictly earlier primary-distance layers
        seen = set()

        best_sec = INF
        best_tert = INF

        i = 0
        while i < n:
            layer_start = i
            curr_primary_dist = dist_vk[nodes[i]][primary]

            # Classify nodes in this layer using only previous layers' info
            while i < n and dist_vk[nodes[i]][primary] == curr_primary_dist:
                v = nodes[i]
                db = dist_vk[v][secondary]
                dc = dist_vk[v][tertiary]

                if db > best_sec or dc > best_tert or (db, dc) in seen:
                    is_dominated[v] = True
                i += 1

            # Now add this layer into history
            for j in range(layer_start, i):
                v = nodes[j]
                db = dist_vk[v][secondary]
                dc = dist_vk[v][tertiary]
                seen.add((db, dc))
                if db < best_sec:
                    best_sec = db
                if dc < best_tert:
                    best_tert = dc

    # If any vertex is not dominated across all permutations, company 1 wins
    company_one_wins = any(not x for x in is_dominated)
    print(1 if company_one_wins else 2)


if __name__ == "__main__":
    solve()
