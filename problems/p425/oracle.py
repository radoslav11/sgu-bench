import sys
from collections import defaultdict

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    # Read matrix
    tbl = [[int(next(it)) for _ in range(m)] for _ in range(n)]

    # Collect all distinct values for required output keys
    values = sorted({tbl[i][j] for i in range(n) for j in range(m)})

    # Build constraint graph:
    # For each row i>0, find first column where it differs from row 0,
    # and add an edge between those two values.
    adj = defaultdict(list)     # value -> list of neighbor values
    degree = defaultdict(int)   # value -> degree

    for i in range(1, n):
        for j in range(m):
            u = tbl[i][j]
            v = tbl[0][j]
            if u != v:
                adj[u].append(v)
                adj[v].append(u)
                degree[u] += 1
                degree[v] += 1
                break  # only one mismatch edge per row is sufficient

    # List only vertices that have constraints (degree > 0), sorted by degree
    vertices = sorted((d, v) for v, d in degree.items())

    # Greedy coloring: assign smallest positive color not used by colored neighbors
    color = {}  # value -> assigned color (positive int)

    for _, v in vertices:
        used = set()
        for u in adj[v]:
            if u in color:
                used.add(color[u])

        c = 1
        while c in used:
            c += 1
        color[v] = c

    # Output format:
    # Always print Yes, then mapping for all distinct values in increasing order.
    out_lines = ["Yes"]
    for v in values:
        out_lines.append(f"{v} -> {color.get(v, 0)}")

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
