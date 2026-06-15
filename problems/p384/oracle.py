import sys

# We will implement the same logic.
# Python does not have stable list iterators like C++ std::list, so O(1) deletion
# by stored iterator is not convenient.
#
# However, in a friendship graph:
# - Every non-center vertex starts with degree 2 (center + partner)
# - Only deletions happen, so degree only decreases.
#
# Therefore we can store adjacency as small sets and delete in O(1) average time.
# Checking edge existence is also O(1) average.
#
# Complexity: O(n + m + q) average, which is fine for this structure.

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    if not data:
        return

    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    edges = []
    adj = [set() for _ in range(n + 1)]

    # Read initial edges, build adjacency sets
    for _ in range(m):
        u = int(next(it)); v = int(next(it))
        edges.append((u, v))
        adj[u].add(v)
        adj[v].add(u)

    # Find center as vertex with maximum degree in the initial graph
    center = 1
    for v in range(2, n + 1):
        if len(adj[v]) > len(adj[center]):
            center = v

    # partner[v] = the non-center neighbor of v in its original triangle
    partner = [0] * (n + 1)
    for v in range(1, n + 1):
        if v == center:
            continue
        # Among neighbors, the one that is not the center is the partner
        for u in adj[v]:
            if u != center:
                partner[v] = u
                break

    def has_edge(u: int, v: int) -> bool:
        # With sets, membership is O(1) average.
        return v in adj[u]

    def dist_to_center(v: int) -> int:
        if v == center:
            return 0
        if has_edge(v, center):
            return 1
        p = partner[v]
        if p != 0 and has_edge(v, p) and has_edge(p, center):
            return 2
        return -1

    def query_length(x: int, y: int) -> int:
        if x == y:
            return 0

        if x == center or y == center:
            other = y if x == center else x
            return dist_to_center(other)

        # Same original triangle (partners)
        if partner[x] == y:
            best = None
            if has_edge(x, y):
                best = 1
            if has_edge(x, center) and has_edge(center, y):
                best = 2 if best is None else min(best, 2)
            return -1 if best is None else best

        # Different triangles: must go through center
        dx = dist_to_center(x)
        dy = dist_to_center(y)
        if dx == -1 or dy == -1:
            return -1
        return dx + dy

    out_lines = []

    # Remaining tokens describe commands until EOF.
    for token in it:
        cmd = token.decode()
        if cmd == "DELETE":
            x = int(next(it)) - 1  # to 0-based
            u, v = edges[x]
            # Remove if present (they always should be, but safe anyway)
            adj[u].discard(v)
            adj[v].discard(u)
        else:  # "LENGTH"
            x = int(next(it))
            y = int(next(it))
            out_lines.append(str(query_length(x, y)))

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
