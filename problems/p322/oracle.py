import sys
from collections import deque

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    n = int(next(it))

    # has1[u][v] tells whether edge (u, v) is in Berland (tree 1)
    # has2[u][v] tells whether edge (u, v) is in Beerland (tree 2), updated as we modify it.
    # Using bytearray for memory efficiency: n<=2000 => matrix is ~4M booleans; ok in Python with bytearrays.
    has1 = [bytearray(n + 1) for _ in range(n + 1)]
    has2 = [bytearray(n + 1) for _ in range(n + 1)]

    # Store Berland edges to iterate later
    edges1 = []

    # Read Berland edges
    for _ in range(n - 1):
        u = int(next(it)); v = int(next(it))
        has1[u][v] = 1
        has1[v][u] = 1
        edges1.append((u, v))

    # Read Beerland edges, build adjacency list
    adj2 = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        u = int(next(it)); v = int(next(it))
        has2[u][v] = 1
        has2[v][u] = 1
        adj2[u].append(v)
        adj2[v].append(u)

    def find_path(start: int, end: int):
        """Return the unique path (as list of vertices) between start and end in current Beerland tree."""
        par = [-1] * (n + 1)
        par[start] = start
        q = deque([start])

        # BFS to set parents
        while q:
            u = q.popleft()
            if u == end:
                break
            for v in adj2[u]:
                if par[v] == -1:
                    par[v] = u
                    q.append(v)

        # Reconstruct path end->start using parents
        path = []
        v = end
        while v != start:
            path.append(v)
            v = par[v]
        path.append(start)
        path.reverse()
        return path

    ops = []  # list of (p, a1, b1, a2, b2)

    for a, b in edges1:
        # If Beerland already has this edge, no action needed
        if has2[a][b]:
            continue

        # Find the path between a and b in Beerland
        path = find_path(a, b)

        # Choose an edge on the path that is NOT in Berland to remove.
        ra = rb = -1
        for i in range(len(path) - 1):
            x = path[i]
            y = path[i + 1]
            if not has1[x][y]:
                ra, rb = x, y
                break

        # Remove (ra, rb) from Beerland adjacency structures
        has2[ra][rb] = 0
        has2[rb][ra] = 0

        # Remove from adjacency lists (O(deg) linear search)
        adj2[ra].remove(rb)
        adj2[rb].remove(ra)

        # Add desired edge (a, b) to Beerland
        has2[a][b] = 1
        has2[b][a] = 1
        adj2[a].append(b)
        adj2[b].append(a)

        # Record operation in the required format; we only modify country 2.
        ops.append((2, a, b, ra, rb))

    out_lines = [str(len(ops))]
    out_lines += ["{} {} {} {} {}".format(*op) for op in ops]
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
