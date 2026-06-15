import sys

# Increase recursion limit because the Hamiltonian path can have up to 100 vertices.
sys.setrecursionlimit(10000)


def solve():
    # Read all input tokens.
    data = sys.stdin.read().split()

    # Pointer into token list.
    ptr = 0

    # Number of facets.
    f = int(data[ptr])
    ptr += 1

    # Maps coordinate triples to vertex ids.
    # Coordinates are kept as strings because equal vertices are printed identically.
    vertex_id = {}

    # vertex_coord[id] stores the original coordinate strings of that vertex.
    vertex_coord = []

    # Set of undirected edges, stored as pairs (min_id, max_id).
    edges = set()

    # Process each facet.
    for _ in range(f):
        # Number of vertices in this facet.
        k = int(data[ptr])
        ptr += 1

        # ids[i] will be the graph id of the i-th vertex of the facet.
        ids = []

        # Read all vertices of this facet.
        for _ in range(k):
            # Read coordinates as strings to preserve exact output formatting.
            x = data[ptr]
            y = data[ptr + 1]
            z = data[ptr + 2]
            ptr += 3

            # Coordinate triple key.
            key = (x, y, z)

            # If this vertex was not seen before, assign it a new id.
            if key not in vertex_id:
                vertex_id[key] = len(vertex_coord)
                vertex_coord.append(key)

            # Store the id of this facet vertex.
            ids.append(vertex_id[key])

        # Add edges between consecutive vertices of the facet boundary.
        for i in range(k):
            a = ids[i]
            b = ids[(i + 1) % k]

            # Normalize undirected edge order.
            if a > b:
                a, b = b, a

            # Add to set to remove duplicates.
            edges.add((a, b))

    # Number of distinct vertices.
    n = len(vertex_coord)

    # Build adjacency list.
    adj = [[] for _ in range(n)]

    # Build adjacency matrix for O(1) edge existence checks.
    connected = [[False] * n for _ in range(n)]

    # Fill graph structures.
    for a, b in edges:
        adj[a].append(b)
        adj[b].append(a)
        connected[a][b] = True
        connected[b][a] = True

    # visited[v] is True if v is already in current path.
    visited = [False] * n

    # path[i] is the i-th vertex in current path.
    path = [0] * n

    # Start Hamiltonian path from vertex 0.
    visited[0] = True
    path[0] = 0

    def dfs(cur, depth):
        """
        Try to extend current Hamiltonian path.

        cur:
            current endpoint of the path

        depth:
            number of vertices currently included in the path
        """

        # If all vertices are used, check whether the path closes into a cycle.
        if depth == n:
            return connected[cur][path[0]]

        # Pruning 1:
        # Every unvisited vertex needs at least two possible neighbors
        # in the final Hamiltonian cycle.
        for u in range(n):
            if visited[u]:
                continue

            # Count usable neighbors of u.
            avail = 0

            for nb in adj[u]:
                # A neighbor is usable if:
                # - it is unvisited,
                # - it is the current endpoint,
                # - it is the start vertex.
                if (not visited[nb]) or nb == cur or nb == path[0]:
                    avail += 1

            # Fewer than two usable neighbors means impossible branch.
            if avail < 2:
                return False

        # Pruning 2:
        # All unvisited vertices must be reachable from cur through unvisited vertices.

        # Number of vertices still not in the path.
        target = n - depth

        # seen[v] marks unvisited vertices reachable from cur.
        seen = [False] * n

        # Stack for DFS over unvisited part of the graph.
        stack = []

        # Start from all unvisited neighbors of current endpoint.
        for nb in adj[cur]:
            if not visited[nb] and not seen[nb]:
                seen[nb] = True
                stack.append(nb)

        # Count reached unvisited vertices.
        cnt = len(stack)

        # DFS through only unvisited vertices.
        while stack:
            x = stack.pop()

            for nb in adj[x]:
                if not visited[nb] and not seen[nb]:
                    seen[nb] = True
                    cnt += 1
                    stack.append(nb)

        # If some unvisited vertex is unreachable, this branch cannot work.
        if cnt != target:
            return False

        # Candidate next vertices.
        # Store pairs (degree, vertex), where degree is number of unvisited neighbors.
        candidates = []

        # Current path can only be extended to an unvisited neighbor of cur.
        for nb in adj[cur]:
            if visited[nb]:
                continue

            # Count unvisited neighbors of nb.
            deg = 0
            for x in adj[nb]:
                if not visited[x]:
                    deg += 1

            candidates.append((deg, nb))

        # Try vertices with fewer options first.
        candidates.sort()

        # Backtracking over candidates.
        for _, nb in candidates:
            # Add nb to current path.
            visited[nb] = True
            path[depth] = nb

            # Recurse.
            if dfs(nb, depth + 1):
                return True

            # Backtrack.
            visited[nb] = False

        # No extension worked.
        return False

    # Run DFS.
    found = dfs(0, 1)

    # Prepare output.
    out = []

    if found:
        out.append("Yes")
        out.append(str(n))

        # Print vertices in Hamiltonian cycle order.
        for v in path:
            x, y, z = vertex_coord[v]
            out.append(f"{x} {y} {z}")
    else:
        out.append("No")

    # Write final answer.
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
