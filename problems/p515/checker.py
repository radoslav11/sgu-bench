"""Checker for p515: usage checker.py <in> <out> <ans>; exit 0 = accept.

The submission outputs the number of roads on a shortest path and then the
road indices (in input order, 1-based) along that path. Any path is accepted
provided it:
  - uses valid, existing road indices forming a connected walk between two
    endpoints (consecutive roads share a vertex);
  - actually has length equal to the shortest-path distance between its two
    endpoints (verified with Dijkstra on the input graph);
  - visits all of the required important cities.

The reference total cost is used only as a sanity reference, never compared
for equality, since multiple valid paths can exist.
"""

import sys
import heapq


def reject(message):
    print(message)
    sys.exit(1)


def main():
    with open(sys.argv[1]) as f:
        tok = f.read().split()
    with open(sys.argv[2]) as f:
        out = f.read().split()

    idx = 0
    n = int(tok[idx]); idx += 1
    m = int(tok[idx]); idx += 1
    edges = []
    adj = [[] for _ in range(n + 1)]
    for i in range(m):
        a = int(tok[idx]); b = int(tok[idx + 1]); t = int(tok[idx + 2])
        idx += 3
        edges.append((a, b, t))
        adj[a].append((b, t))
        adj[b].append((a, t))
    k = int(tok[idx]); idx += 1
    important = [int(tok[idx + j]) for j in range(k)]
    idx += k

    if not out:
        reject("empty output")
    cnt = int(out[0])
    road_ids = [int(x) for x in out[1:]]
    if len(road_ids) != cnt:
        reject(f"declared {cnt} roads, listed {len(road_ids)}")

    for rid in road_ids:
        if not (1 <= rid <= m):
            reject(f"road index {rid} out of range")

    # Reconstruct the vertex sequence of the walk from the edge list.
    visited_nodes = set()
    path_cost = 0
    if cnt == 0:
        # Zero-length path: it is a single vertex. Only valid if there is at
        # most one important city (a single node trivially covers it).
        if k > 1:
            reject("empty path cannot cover multiple important cities")
        # The single covered vertex is unconstrained here; if k==1 we cannot
        # know which vertex, so accept only when k==0 or k==1 with that city
        # reachable trivially (path of one node = that city).
        sys.exit(0)

    # Determine the starting orientation using the first edge.
    a0, b0, _ = edges[road_ids[0] - 1]
    if cnt == 1:
        start = a0
    else:
        a1, b1, _ = edges[road_ids[1] - 1]
        # Pick the endpoint of edge0 that is NOT shared with edge1 as start.
        if a0 in (a1, b1):
            start = b0
        elif b0 in (a1, b1):
            start = a0
        else:
            reject("first two roads are not adjacent")

    seq = [start]
    cur = start
    for rid in road_ids:
        a, b, t = edges[rid - 1]
        if cur == a:
            nxt = b
        elif cur == b:
            nxt = a
        else:
            reject(f"road {rid} ({a},{b}) is not incident to current node {cur}")
        path_cost += t
        seq.append(nxt)
        cur = nxt

    for v in seq:
        visited_nodes.add(v)

    for c in important:
        if c not in visited_nodes:
            reject(f"important city {c} not visited by the path")

    s, t_end = seq[0], seq[-1]

    # Dijkstra from s to confirm the path is a genuine shortest path.
    INF = float("inf")
    dist = [INF] * (n + 1)
    dist[s] = 0
    pq = [(0, s)]
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v, w in adj[u]:
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                heapq.heappush(pq, (dist[v], v))

    if dist[t_end] != path_cost:
        reject(
            f"path cost {path_cost} is not the shortest distance "
            f"{dist[t_end]} between endpoints {s} and {t_end}"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
