import sys
sys.setrecursionlimit(10000)

def read_input():
    """Reads N and adjacency lists, builds edge list and per-vertex input order."""
    N = int(sys.stdin.readline())
    input_order = [[] for _ in range(N)]
    edge_id = {}      # map (u,v)->eid
    edges = []        # list of (u,v)
    deg = [0]*N

    # Build the graph, assigning each undirected edge a unique eid
    for u in range(N):
        for x in map(int, sys.stdin.readline().split()):
            if x == 0: break
            v = x-1
            key = (u,v)
            if key in edge_id:
                eid = edge_id[key]
            else:
                eid = len(edges)
                edges.append((u,v))
                edge_id[(u,v)] = edge_id[(v,u)] = eid
                deg[u] += 1
                deg[v] += 1
            input_order[u].append(eid)
    return N, edges, deg, input_order

def build_euler(N, edges, deg):
    """Construct adjacency with real edges, then add fake edges to pair odd vertices."""
    m = len(edges)
    adj = [[] for _ in range(N)]  # (neighbor, eid2)
    # real edges
    for eid, (u,v) in enumerate(edges):
        adj[u].append((v, eid*2))
        adj[v].append((u, eid*2+1))
    # pair odd-degree vertices
    odd = [u for u in range(N) if deg[u] % 2 == 1]
    fake_start = m
    for i in range(0, len(odd), 2):
        u, v = odd[i], odd[i+1]
        eid2a = 2*(fake_start)     # ≥ 2*m flags fake
        eid2b = eid2a + 1
        adj[u].append((v, eid2a))
        adj[v].append((u, eid2b))
        fake_start += 1
    return adj, fake_start

def hierholzer(adj, total_eid2, m_real):
    """Extract Eulerian circuits; return list of trails (lists of eid2).

    Fake pairing edges (eid2 >= 2*m_real) split a circuit into separate
    trails so that colour alternation restarts after each fake edge.
    """
    used = [False]* (total_eid2//2)
    ptr = [0]*len(adj)
    trails = []

    def dfs(u, out):
        while ptr[u] < len(adj[u]):
            v, eid2 = adj[u][ptr[u]]
            ptr[u] += 1
            eid = eid2>>1
            if not used[eid]:
                used[eid] = True
                dfs(v, out)
                out.append(eid2)

    # For each vertex with edges, peel off a circuit
    for u in range(len(adj)):
        if ptr[u] < len(adj[u]):
            path = []
            dfs(u, path)
            if not path:
                continue
            # rotate so if fake edge exists we start there
            m = m_real
            for i, x in enumerate(path):
                if x >= 2*m:
                    path = path[i:]+path[:i]
                    break
            # split at fake edges
            cur = []
            for x in path:
                if x < 2*m:
                    cur.append(x)
                elif cur:
                    trails.append(cur)
                    cur = []
            if cur:
                trails.append(cur)
    return trails

def solve():
    N, edges, deg, input_order = read_input()
    adj, fake_limit = build_euler(N, edges, deg)
    trails = hierholzer(adj, fake_limit*2, len(edges))

    m = len(edges)
    color = [-1]*m           # final color of each real edge
    mask = [0]*N            # bitmask per vertex

    # Color each trail alternately
    for tr in trails:
        c = 1
        for eid2 in tr:
            eid = eid2>>1
            if eid < m:
                color[eid] = c
                u,v = edges[eid]
                if (eid2 & 1)==1:
                    u,v = v,u
                mask[u] |= 1<<c
                mask[v] |= 1<<c
                c = 3-c

    # Verify all vertices with deg>=2 have both colors
    need = (1<<1)|(1<<2)
    for u in range(N):
        if deg[u]>=2 and mask[u]!=need:
            print("No solution")
            return

    # Print in input order
    out = []
    for u in range(N):
        line = []
        for eid in input_order[u]:
            line.append(str(color[eid]))
        line.append('0')
        out.append(' '.join(line))
    print('\n'.join(out))

if __name__ == "__main__":
    solve()
