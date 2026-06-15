import sys

sys.setrecursionlimit(1_000_000)


def solve() -> None:
    input = sys.stdin.readline
    n, m = map(int, input().split())

    # all_edges: full graph G (all edges regardless of weight)
    all_edges = [[] for _ in range(n)]

    # zero-edge graph H for SCC: only edges with w == 0 and u != v
    adj = [[] for _ in range(n)]
    radj = [[] for _ in range(n)]

    # remember zero self-loops separately
    has_zero_self_loop = [False] * n

    for _ in range(m):
        u, v, w = map(int, input().split())
        u -= 1
        v -= 1

        # store edge in full graph
        all_edges[u].append(v)

        if w == 0:
            if u == v:
                has_zero_self_loop[u] = True
            else:
                adj[u].append(v)
                radj[v].append(u)

    # --- Kosaraju SCC on H (zero edges) ---
    visited = [False] * n
    order = []

    def dfs1(u: int) -> None:
        """First pass DFS to compute finishing order."""
        visited[u] = True
        for v in adj[u]:
            if not visited[v]:
                dfs1(v)
        order.append(u)

    for i in range(n):
        if not visited[i]:
            dfs1(i)

    comp = [-1] * n
    comp_count = 0

    def dfs2(u: int, cid: int) -> None:
        """Second pass DFS on reversed graph to assign component id."""
        comp[u] = cid
        for v in radj[u]:
            if comp[v] == -1:
                dfs2(v, cid)

    # process in reverse finishing order
    for u in reversed(order):
        if comp[u] == -1:
            dfs2(u, comp_count)
            comp_count += 1

    # component sizes
    comp_size = [0] * comp_count
    for u in range(n):
        comp_size[comp[u]] += 1

    # alive[u] = 1 means alive, 0 means not alive
    alive = [1] * n

    # initial "bad" vertices:
    # - zero self-loop
    # - belongs to SCC with size > 1 in zero-edge graph (zero cycle)
    for u in range(n):
        if has_zero_self_loop[u] or comp_size[comp[u]] > 1:
            alive[u] = 0

    # propagate "not alive" along the full graph
    def propagate(u: int) -> None:
        """If u is not alive, mark all reachable vertices as not alive."""
        for v in all_edges[u]:
            if alive[v] == 1:
                alive[v] = 0
                propagate(v)

    for u in range(n):
        if alive[u] == 0:
            propagate(u)

    sys.stdout.write("\n".join(map(str, alive)))


if __name__ == "__main__":
    solve()
