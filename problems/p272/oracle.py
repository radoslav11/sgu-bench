import sys
from collections import deque


def main():
    # Read all integers from standard input at once for speed.
    data = list(map(int, sys.stdin.buffer.read().split()))

    # Pointer into the input array.
    idx = 0

    # Read number of vertices and tunnels.
    n = data[idx]
    idx += 1
    m = data[idx]
    idx += 1

    # Original undirected graph adjacency list.
    # Vertices are numbered from 1 to n.
    adj = [[] for _ in range(n + 1)]

    # Store the original tunnel list as pairs.
    tunnels = []

    # Read all tunnels.
    for _ in range(m):
        u = data[idx]
        idx += 1
        v = data[idx]
        idx += 1

        tunnels.append((u, v))

        # Undirected graph: add both directions.
        adj[u].append(v)
        adj[v].append(u)

    # Read set A.
    n1 = data[idx]
    idx += 1
    a_set = data[idx:idx + n1]
    idx += n1

    # Read set B.
    n2 = data[idx]
    idx += 1
    b_set = data[idx:idx + n2]
    idx += n2

    # Large value representing infinity.
    INF = 10 ** 18

    # dist[v] = distance from v to nearest exit in B.
    dist = [INF] * (n + 1)

    # Multi-source BFS queue.
    q = deque()

    # Initialize BFS with all exits.
    for b in b_set:
        if dist[b] == INF:
            dist[b] = 0
            q.append(b)

    # Standard BFS.
    while q:
        u = q.popleft()

        for v in adj[u]:
            if dist[v] == INF:
                dist[v] = dist[u] + 1
                q.append(v)

    # K is the shortest distance from any A-lab to any B-lab.
    k = min(dist[a] for a in a_set)

    # Node ids in the auxiliary split graph:
    # source = 0
    # in_node(v) = v
    # out_node(v) = n + v
    # sink = 2 * n + 1
    src = 0
    snk = 2 * n + 1
    total_nodes = 2 * n + 2

    def in_node(v):
        # Input side of original vertex v.
        return v

    def out_node(v):
        # Output side of original vertex v.
        return n + v

    # Each edge is stored as [to, capacity, flow].
    edges = []

    # g[u] contains indices of outgoing edges from u.
    g = [[] for _ in range(total_nodes)]

    def add_edge(u, v, cap):
        # Add edge index to adjacency list.
        g[u].append(len(edges))

        # Store directed edge.
        edges.append([v, cap, 0])

    # Add source edges to usable A-labs.
    # Only A-labs with dist[a] == k can start a length-k shortest path.
    for a in a_set:
        if dist[a] == k:
            add_edge(src, in_node(a), 1)

    # Add vertex-capacity edges.
    # Each original vertex can be used by at most one evacuation path.
    for v in range(1, n + 1):
        add_edge(in_node(v), out_node(v), 1)

    # Add directed edges along shortest paths toward exits.
    for u, v in tunnels:
        # If u is exactly one step farther from exits than v,
        # then a shortest path may go u -> v.
        if dist[u] != INF and dist[u] == dist[v] + 1:
            add_edge(out_node(u), in_node(v), 1)

        # If v is exactly one step farther from exits than u,
        # then a shortest path may go v -> u.
        elif dist[v] != INF and dist[v] == dist[u] + 1:
            add_edge(out_node(v), in_node(u), 1)

    # Add exit edges to sink.
    for b in b_set:
        add_edge(out_node(b), snk, 1)

    # Current edge pointer for each auxiliary node.
    ptr = [0] * total_nodes

    # Recursion depth may be up to the path length in the split graph.
    sys.setrecursionlimit(10 ** 6)

    def dfs(u):
        """
        Try to find one more available path from u to the sink.

        This is a greedy DFS with current-arc optimization.
        It does not use reverse edges because we only need a maximal,
        not maximum, set of paths.
        """

        # Sink reached: path found.
        if u == snk:
            return True

        # Try outgoing edges starting from the saved pointer.
        while ptr[u] < len(g[u]):
            eid = g[u][ptr[u]]
            to, cap, flow = edges[eid]

            # If this edge has remaining capacity and the destination
            # can reach the sink, use this edge.
            if cap - flow > 0 and dfs(to):
                edges[eid][2] += 1
                return True

            # This edge cannot help anymore; skip it forever.
            ptr[u] += 1

        # No outgoing edge leads to the sink.
        return False

    # Repeatedly add one more path while possible.
    while dfs(src):
        pass

    # Reconstruct actual paths from saturated source edges.
    paths = []

    for eid in g[src]:
        # Only used source edges correspond to selected evacuation paths.
        if edges[eid][2] == 0:
            continue

        # The target of source edge is a_in.
        # Since in_node(a) == a, it is also the original vertex number.
        cur = edges[eid][0]

        path = []

        while True:
            # Add current original vertex.
            path.append(cur)

            # dist == 0 means this vertex is in B, so path ends here.
            if dist[cur] == 0:
                break

            # Move along the unique saturated shortest-path edge from cur.
            next_cur = None

            for out_eid in g[out_node(cur)]:
                to, cap, flow = edges[out_eid]

                # Ignore edge to sink; we only want movement to next lab.
                if flow == 1 and to != snk:
                    next_cur = to
                    break

            # This should always exist for a valid saturated path.
            cur = next_cur

    # Store reconstructed path.
        paths.append(path)

    # Prepare output.
    output = []

    # First line: number of paths and common length.
    output.append(f"{len(paths)} {k}")

    # Each path contains k + 1 vertices.
    for path in paths:
        output.append(" ".join(map(str, path)))

    # Print final answer.
    sys.stdout.write("\n".join(output))


if __name__ == "__main__":
    main()
