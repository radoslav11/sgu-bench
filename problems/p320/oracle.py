import sys


def solve():
    # Read all input lines.
    data = sys.stdin.read().strip().split()

    # If input is empty, do nothing.
    if not data:
        return

    # First three tokens are N, M, K.
    n = int(data[0])
    m = int(data[1])
    k = int(data[2])

    # Next n tokens are grid rows.
    rows = data[3:3 + n]

    # Flatten the grid into one string for simpler indexing.
    # Cell (r, c) has flat index r * m + c.
    flat = ''.join(rows)

    # Total number of cells.
    total = n * m

    # comp_id[idx] is the component ID of cell idx.
    comp_id = [-1] * total

    # Size of each component.
    comp_size = []

    # Whether each component touches the grid border.
    touches_border = []

    # Four movement directions.
    directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    # Flood-fill all same-digit connected components.
    for start in range(total):
        # Skip already labeled cells.
        if comp_id[start] != -1:
            continue

        # New component ID.
        cid = len(comp_size)

        # Digit/color of this component.
        color = flat[start]

        # Mark starting cell.
        comp_id[start] = cid

        # Stack for iterative DFS flood fill.
        stack = [start]

        # Component size counter.
        size = 0

        # Whether this component touches the border.
        border = False

        # Process all cells in this component.
        while stack:
            # Pop one cell.
            v = stack.pop()

            # Count it.
            size += 1

            # Convert flat index to row and column.
            r = v // m
            c = v % m

            # Check whether it is on the border.
            if r == 0 or r == n - 1 or c == 0 or c == m - 1:
                border = True

            # Explore four neighbors.
            for dr, dc in directions:
                nr = r + dr
                nc = c + dc

                # Ignore outside-grid positions.
                if nr < 0 or nr >= n or nc < 0 or nc >= m:
                    continue

                # Neighbor flat index.
                to = nr * m + nc

                # Same digit and unvisited means it belongs to this component.
                if flat[to] == color and comp_id[to] == -1:
                    comp_id[to] = cid
                    stack.append(to)

        # Store component information.
        comp_size.append(size)
        touches_border.append(border)

    # Number of components.
    num_comps = len(comp_size)

    # Build component graph.
    outside = num_comps
    vertex_count = num_comps + 1

    # Use a set to avoid duplicate edges between the same components.
    edge_set = set()

    # Scan horizontal and vertical neighboring cell pairs.
    for r in range(n):
        base = r * m
        for c in range(m):
            idx = base + c
            a = comp_id[idx]

            # Right neighbor.
            if c + 1 < m:
                b = comp_id[idx + 1]
                if a != b:
                    x, y = (a, b) if a < b else (b, a)
                    edge_set.add(x * num_comps + y)

            # Down neighbor.
            if r + 1 < n:
                b = comp_id[idx + m]
                if a != b:
                    x, y = (a, b) if a < b else (b, a)
                    edge_set.add(x * num_comps + y)

    # Adjacency list including OUTSIDE vertex.
    adj = [[] for _ in range(vertex_count)]

    # Add component-component edges.
    for code in edge_set:
        a = code // num_comps
        b = code % num_comps
        adj[a].append(b)
        adj[b].append(a)

    # Connect border-touching components to OUTSIDE.
    for c in range(num_comps):
        if touches_border[c]:
            adj[c].append(outside)
            adj[outside].append(c)

    # Mark big components.
    is_big = [size >= k for size in comp_size]

    # Tarjan arrays.
    disc = [-1] * vertex_count
    low = [0] * vertex_count

    # Difference array over DFS order.
    diff = [0] * (vertex_count + 2)

    # DFS timer.
    timer = 0

    # Start DFS from OUTSIDE.
    disc[outside] = low[outside] = timer
    timer += 1

    # Stack frame format:
    # [vertex, next_neighbor_index, parent]
    stack = [[outside, 0, -1]]

    # Iterative Tarjan DFS.
    while stack:
        u, it, parent = stack[-1]

        # If there are still neighbors to process.
        if it < len(adj[u]):
            # Take next neighbor.
            v = adj[u][it]

            # Advance iterator in the top stack frame.
            stack[-1][1] += 1

            # Ignore tree edge to parent.
            if v == parent:
                continue

            # Tree edge to an unvisited vertex.
            if disc[v] == -1:
                disc[v] = low[v] = timer
                timer += 1
                stack.append([v, 0, u])

            # Back edge to an already visited vertex.
            else:
                low[u] = min(low[u], disc[v])

        # All neighbors processed; finish u.
        else:
            # Entry time of u's subtree.
            u_in = disc[u]

            # Since DFS preorder intervals are contiguous,
            # the last discovered vertex so far is subtree end.
            u_out = timer - 1

            # Final low-link value.
            u_low = low[u]

            # Remove u from stack.
            stack.pop()

            # OUTSIDE has no parent.
            if parent == -1:
                continue

            # Propagate low-link value upward.
            low[parent] = min(low[parent], u_low)

            # If parent is a big component and separates u's subtree,
            # mark that whole subtree as dangerous.
            if parent < num_comps and is_big[parent] and u_low >= disc[parent]:
                diff[u_in] += 1
                diff[u_out + 1] -= 1

    # Prefix sum of difference array.
    for i in range(1, timer):
        diff[i] += diff[i - 1]

    # Count dangerous cells.
    answer = 0

    for c in range(num_comps):
        # Component is dangerous if it is big or dominated by a big separator.
        if is_big[c] or diff[disc[c]] > 0:
            answer += comp_size[c]

    # Output answer.
    print(answer)


if __name__ == "__main__":
    solve()
