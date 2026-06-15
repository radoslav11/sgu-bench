import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    # Build adjacency list for a directed graph on n nodes
    adj = [[] for _ in range(n)]
    indeg = [0] * n

    for _ in range(m):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        # u lighter than v => edge u->v
        adj[u].append(v)
        indeg[v] += 1

    # Kahn's algorithm for topological sorting
    q = deque([u for u in range(n) if indeg[u] == 0])
    order = []

    while q:
        u = q.popleft()
        order.append(u)
        for v in adj[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                q.append(v)

    # If we did not process all nodes, there is a cycle => no solution
    if len(order) != n:
        print("No solution")
        return

    # pos[u] = index of node u in the topological order
    pos = [0] * n
    for idx, u in enumerate(order):
        pos[u] = idx

    # Assign weights 1..n in topo order, so box u gets pos[u]+1
    result = [str(pos[u] + 1) for u in range(n)]
    print(" ".join(result))


if __name__ == "__main__":
    main()
