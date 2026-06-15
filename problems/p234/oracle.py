import sys
from collections import deque


def hopcroft_karp(adj, left_size, right_size):
    """
    Computes maximum bipartite matching.

    adj[u] contains all right-side vertices adjacent to left-side vertex u.

    Returns:
        pair_u: pair_u[u] = matched right vertex of left u, or -1
        pair_v: pair_v[v] = matched left vertex of right v, or -1
    """

    pair_u = [-1] * left_size      # Matching from left to right.
    pair_v = [-1] * right_size     # Matching from right to left.
    dist = [-1] * left_size        # BFS levels for left vertices.

    def bfs():
        """
        Builds BFS layers starting from all unmatched left vertices.
        Returns True if there may be an augmenting path.
        """

        q = deque()                # Queue for BFS.

        for u in range(left_size):
            if pair_u[u] == -1:    # Unmatched left vertices are sources.
                dist[u] = 0
                q.append(u)
            else:
                dist[u] = -1       # Matched vertices initially unvisited.

        found = False              # Whether we reached an unmatched right vertex.

        while q:
            u = q.popleft()

            for v in adj[u]:
                matched_left = pair_v[v]

                if matched_left == -1:
                    # We found an edge to an unmatched right vertex,
                    # so an augmenting path exists.
                    found = True
                elif dist[matched_left] == -1:
                    # Continue along the alternating path.
                    dist[matched_left] = dist[u] + 1
                    q.append(matched_left)

        return found

    def dfs(u):
        """
        Attempts to find an augmenting path from left vertex u,
        respecting BFS layers.
        """

        for v in adj[u]:
            matched_left = pair_v[v]

            if matched_left == -1 or (
                dist[matched_left] == dist[u] + 1 and dfs(matched_left)
            ):
                pair_u[u] = v
                pair_v[v] = u
                return True

        # Mark as unusable in this phase.
        dist[u] = -1
        return False

    matching_size = 0

    while bfs():
        for u in range(left_size):
            if pair_u[u] == -1:
                if dfs(u):
                    matching_size += 1

    return pair_u, pair_v


def maximum_independent_set(adj, pair_u, pair_v, left_size, right_size):
    """
    Given a maximum matching, computes a maximum independent set
    in a bipartite graph.

    Uses König's theorem:
        maximum independent set = complement of minimum vertex cover.

    Minimum vertex cover is obtained by alternating BFS from unmatched
    left vertices.
    """

    visited_left = [False] * left_size
    visited_right = [False] * right_size

    q = deque()

    # Start from all unmatched left vertices.
    for u in range(left_size):
        if pair_u[u] == -1:
            visited_left[u] = True
            q.append(u)

    while q:
        u = q.popleft()

        # From left side, traverse only non-matching edges.
        for v in adj[u]:
            if pair_u[u] == v:
                continue

            if not visited_right[v]:
                visited_right[v] = True

                # From right side, traverse the matching edge if it exists.
                matched_left = pair_v[v]
                if matched_left != -1 and not visited_left[matched_left]:
                    visited_left[matched_left] = True
                    q.append(matched_left)

    # Minimum vertex cover:
    #   left not visited + right visited
    #
    # Maximum independent set is complement:
    #   left visited + right not visited
    left_set = [u for u in range(left_size) if visited_left[u]]
    right_set = [v for v in range(right_size) if not visited_right[v]]

    return left_set, right_set


def main():
    data = sys.stdin.read().split()

    if not data:
        return

    it = iter(data)

    m = int(next(it))  # Number of rows.
    n = int(next(it))  # Number of columns.

    grid = [[0] * n for _ in range(m)]

    for i in range(m):
        for j in range(n):
            grid[i][j] = int(next(it))

    # id_cell[i][j] stores the id of this cell in its bipartite side.
    id_cell = [[-1] * n for _ in range(m)]

    left_count = 0
    right_count = 0

    # Assign IDs to existing cells according to checkerboard parity.
    for i in range(m):
        for j in range(n):
            if grid[i][j] == 1:
                if (i + j) % 2 == 0:
                    id_cell[i][j] = left_count
                    left_count += 1
                else:
                    id_cell[i][j] = right_count
                    right_count += 1

    # Adjacency list from even-parity cells to odd-parity cells.
    adj = [[] for _ in range(left_count)]

    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    # Build graph edges between neighboring existing cells.
    for i in range(m):
        for j in range(n):
            if grid[i][j] == 1 and (i + j) % 2 == 0:
                u = id_cell[i][j]

                for di, dj in directions:
                    ni = i + di
                    nj = j + dj

                    if 0 <= ni < m and 0 <= nj < n and grid[ni][nj] == 1:
                        v = id_cell[ni][nj]
                        adj[u].append(v)

    # Python recursion depth may need to be increased for DFS.
    sys.setrecursionlimit(1_000_000)

    # Compute maximum matching.
    pair_u, pair_v = hopcroft_karp(adj, left_count, right_count)

    # Compute maximum independent set.
    left_set, right_set = maximum_independent_set(
        adj, pair_u, pair_v, left_count, right_count
    )

    chosen_left = [False] * left_count
    chosen_right = [False] * right_count

    for u in left_set:
        chosen_left[u] = True

    for v in right_set:
        chosen_right[v] = True

    # Build output grid.
    output = [['#'] * n for _ in range(m)]

    guardian_count = len(left_set) + len(right_set)

    for i in range(m):
        for j in range(n):
            if grid[i][j] == 0:
                output[i][j] = '#'
            else:
                if (i + j) % 2 == 0:
                    chosen = chosen_left[id_cell[i][j]]
                else:
                    chosen = chosen_right[id_cell[i][j]]

                output[i][j] = 'G' if chosen else '.'

    result = [str(guardian_count)]

    for row in output:
        result.append(''.join(row))

    sys.stdout.write('\n'.join(result))


if __name__ == "__main__":
    main()
