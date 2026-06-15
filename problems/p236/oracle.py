import sys
sys.setrecursionlimit(10**7)

def readints():
    return map(int, sys.stdin.readline().split())

def find_cycle(n, edges, x):
    """
    Run Bellman-Ford to detect a negative cycle under weights w' = x*t - c.
    Returns (has_cycle, list_of_nodes_in_cycle).
    """
    INF = 1e18
    dist = [INF] * (n + 2)
    par  = [-1]  * (n + 2)
    source = n + 1
    dist[source] = 0.0

    # Relax edges n times
    for _ in range(n):
        # Relax real edges
        for u, v, c, t in edges:
            w = x*t - c
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                par[v] = u
        # Relax super-source → every node
        for v in range(1, n+1):
            if dist[source] < dist[v]:
                dist[v] = dist[source]
                par[v] = source

    # One extra pass to detect a negative cycle
    cycle_node = -1
    for u, v, c, t in edges:
        w = x*t - c
        if dist[u] + w < dist[v]:
            cycle_node = v
            break

    if cycle_node == -1:
        return False, []

    # Walk back n steps to ensure inside the cycle
    for _ in range(n):
        cycle_node = par[cycle_node]

    # Reconstruct cycle
    cycle = []
    curr = cycle_node
    while True:
        cycle.append(curr)
        curr = par[curr]
        if curr == cycle_node:
            break
    cycle.reverse()
    return True, cycle

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n, m = map(int, (next(it), next(it)))
    edges = []
    for _ in range(m):
        u, v, c, t = map(int, (next(it), next(it), next(it), next(it)))
        edges.append((u, v, c, t))

    lo, hi = 0.0, 200.0
    # Binary search for ~80–100 iterations
    for _ in range(80):
        mid = (lo + hi) / 2.0
        has, _ = find_cycle(n, edges, mid)
        if has:
            lo = mid
        else:
            hi = mid

    # Recover the cycle at best ratio lo
    has, cycle = find_cycle(n, edges, lo)
    if not has:
        print(0)
    else:
        print(len(cycle))
        print(" ".join(map(str, cycle)))

if __name__ == "__main__":
    main()
