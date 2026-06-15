from collections import deque
import sys


def main():
    # Read all integers from input.
    data = list(map(int, sys.stdin.read().split()))

    # First two integers are N and M.
    n, m = data[0], data[1]

    # Adjacency matrix with multiplicities.
    # g[a][b] = number of edges/swaps between a and b.
    g = [[0] * (n + 1) for _ in range(n + 1)]

    idx = 2

    # Read M swap operations.
    for _ in range(m):
        a = data[idx]
        b = data[idx + 1]
        idx += 2

        # Add undirected multiedge.
        g[a][b] += 1
        g[b][a] += 1

    def reaches(src, dst):
        """
        Return True if dst is reachable from src in the current graph.
        """
        visited = [False] * (n + 1)
        q = deque([src])
        visited[src] = True

        while q:
            v = q.popleft()

            # Scan all possible neighbors because we use adjacency matrix.
            for w in range(1, n + 1):
                if g[v][w] > 0 and not visited[w]:
                    visited[w] = True
                    q.append(w)

        return visited[dst]

    def bfs_dist(src):
        """
        Return shortest path distances from src.
        dist[v] == -1 means unreachable.
        """
        dist = [-1] * (n + 1)
        q = deque([src])
        dist[src] = 0

        while q:
            v = q.popleft()

            for w in range(1, n + 1):
                if g[v][w] > 0 and dist[w] == -1:
                    dist[w] = dist[v] + 1
                    q.append(w)

        return dist

    def has_cycle_through(v):
        """
        Check whether there is a cycle passing through vertex v.

        For every incident edge v-z, remove one copy.
        If v can still reach z, then that path plus the removed edge
        forms a cycle through v.
        """
        for z in range(1, n + 1):
            # Skip self and non-neighbors.
            if z == v or g[v][z] == 0:
                continue

            # Remove one copy of edge v-z.
            g[v][z] -= 1
            g[z][v] -= 1

            # If v still reaches z, the removed edge lies on a cycle.
            ok = reaches(v, z)

            # Restore the edge.
            g[v][z] += 1
            g[z][v] += 1

            if ok:
                return True

        return False

    def in_large_2ec(v):
        """
        Check whether v belongs to a bridge-free component of size >= 3.

        Such a component allows a closed trail through v visiting at least
        three distinct vertices.
        """
        bridge = [[False] * (n + 1) for _ in range(n + 1)]

        # Detect bridges naively.
        for a in range(1, n + 1):
            for b in range(a + 1, n + 1):
                # Only single edges can be bridges.
                # Parallel edges cannot be bridges.
                if g[a][b] != 1:
                    continue

                # Temporarily remove edge a-b.
                g[a][b] = 0
                g[b][a] = 0

                # If endpoints become disconnected, it is a bridge.
                if not reaches(a, b):
                    bridge[a][b] = True
                    bridge[b][a] = True

                # Restore edge.
                g[a][b] = 1
                g[b][a] = 1

        # BFS using only non-bridge edges.
        visited = [False] * (n + 1)
        q = deque([v])
        visited[v] = True
        count = 1

        while q:
            u = q.popleft()

            for w in range(1, n + 1):
                if g[u][w] > 0 and not bridge[u][w] and not visited[w]:
                    visited[w] = True
                    count += 1
                    q.append(w)

        return count >= 3

    # Distances from starting position 1.
    dist = bfs_dist(1)

    answer = []

    # Test every possible final position.
    for u in range(1, n + 1):
        # Unreachable vertices are impossible.
        if dist[u] == -1:
            continue

        # Special case: final position is 1.
        if u == 1:
            # Degree of vertex 1, counting multiplicities.
            deg = sum(g[1][w] for w in range(1, n + 1))

            # If no swap touches 1, the ball can remain at 1.
            if deg == 0:
                answer.append(u)
                continue

            # Check for even multiplicity edge {1, w}.
            even_multi = False
            for w in range(1, n + 1):
                if g[1][w] >= 2 and g[1][w] % 2 == 0:
                    even_multi = True
                    break

            # Either a two-vertex even oscillation works,
            # or a larger closed trail through 1 works.
            if even_multi or in_large_2ec(1):
                answer.append(u)

        # If u is at distance >= 2, a path to it visits at least 3 vertices.
        elif dist[u] >= 2:
            answer.append(u)

        # Remaining case: u is directly adjacent to 1.
        else:
            # Multiplicity of edge {1, u}.
            x = g[1][u]

            # Remove all direct edges between 1 and u.
            g[1][u] = 0
            g[u][1] = 0

            # Alternative path from 1 to u gives a 3-vertex trail.
            ok = reaches(1, u)

            # If no alternative path, using all direct edges ends at u
            # exactly when their count is odd.
            if not ok and x % 2 == 1:
                ok = True

            # If direct count is even, we need a cycle on either side
            # to visit a third vertex.
            if not ok and (has_cycle_through(1) or has_cycle_through(u)):
                ok = True

            # Restore direct edges.
            g[1][u] = x
            g[u][1] = x

            if ok:
                answer.append(u)

    # Output positions in any order.
    print(*answer)


if __name__ == "__main__":
    main()
