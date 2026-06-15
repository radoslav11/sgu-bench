import sys
from collections import deque

# Solve using MKM blocking flow on a layered DAG.
# Complexity: O(E + V^2), fits N<=1500, M<=300000.

def solve() -> None:
    sys.setrecursionlimit(1_000_000)
    input = sys.stdin.readline

    n, m, L = map(int, input().split())
    lev = [0] * (n + 1)
    out_edges = [[] for _ in range(n + 1)]  # store edge ids
    in_edges  = [[] for _ in range(n + 1)]
    by_level = [[] for _ in range(L + 1)]

    levels = list(map(int, input().split()))
    src = snk = -1
    for i in range(1, n + 1):
        lev[i] = levels[i - 1]
        if lev[i] == 1:
            src = i
        if lev[i] == L:
            snk = i
        by_level[lev[i]].append(i)

    # Edge arrays (structure-of-arrays for speed in Python)
    frm = [0] * m
    to  = [0] * m
    cap = [0] * m
    flow = [0] * m

    for eid in range(m):
        a, b, c = map(int, input().split())
        frm[eid] = a
        to[eid] = b
        cap[eid] = c
        out_edges[a].append(eid)
        in_edges[b].append(eid)

    # Residual sums per node
    out_sum = [0] * (n + 1)
    in_sum = [0] * (n + 1)
    for eid in range(m):
        out_sum[frm[eid]] += cap[eid]
        in_sum[to[eid]] += cap[eid]

    out_ptr = [0] * (n + 1)
    in_ptr  = [0] * (n + 1)
    alive = [True] * (n + 1)

    def pot(v: int) -> int:
        """Node potential as in MKM."""
        if v == src:
            return out_sum[v]
        if v == snk:
            return in_sum[v]
        # internal node: limited by both in and out residual
        return in_sum[v] if in_sum[v] < out_sum[v] else out_sum[v]

    dead_q = deque()

    def try_kill(v: int) -> None:
        """If v is an internal alive node with potential 0, mark it for removal."""
        if alive[v] and v != src and v != snk and pot(v) == 0:
            dead_q.append(v)

    def kill_dead() -> None:
        """Remove all nodes with pot==0, cascading updates to neighbors' sums."""
        while dead_q:
            u = dead_q.popleft()
            if not alive[u]:
                continue
            alive[u] = False

            # Removing u disables residual u->x edges, so x loses incoming residual.
            for eid in out_edges[u]:
                res = cap[eid] - flow[eid]
                if res > 0:
                    v = to[eid]
                    in_sum[v] -= res
                    try_kill(v)

            # Removing u disables residual x->u edges, so x loses outgoing residual.
            for eid in in_edges[u]:
                res = cap[eid] - flow[eid]
                if res > 0:
                    v = frm[eid]
                    out_sum[v] -= res
                    try_kill(v)

    for i in range(1, n + 1):
        try_kill(i)
    kill_dead()

    buf = [0] * (n + 1)

    while True:
        # Find alive node with minimal positive potential
        v = -1
        min_p = 10**30
        for i in range(1, n + 1):
            if alive[i]:
                p = pot(i)
                if 0 < p < min_p:
                    min_p = p
                    v = i
        if v == -1:
            break

        p = min_p

        # ---- push forward from v to sink ----
        for i in range(1, n + 1):
            buf[i] = 0
        buf[v] = p

        for level in range(lev[v], L):
            for u in by_level[level]:
                if (not alive[u]) or buf[u] == 0:
                    continue

                # scan outgoing edges with persistent pointer
                ptr = out_ptr[u]
                adj = out_edges[u]
                while ptr < len(adj) and buf[u] > 0:
                    eid = adj[ptr]
                    res = cap[eid] - flow[eid]
                    w = to[eid]
                    if res <= 0 or (not alive[w]):
                        ptr += 1
                        continue

                    f = buf[u] if buf[u] < res else res
                    flow[eid] += f
                    buf[u] -= f
                    buf[w] += f
                    out_sum[u] -= f
                    in_sum[w] -= f

                    if flow[eid] == cap[eid]:
                        ptr += 1
                out_ptr[u] = ptr

        # ---- push backward from v to source ----
        for i in range(1, n + 1):
            buf[i] = 0
        buf[v] = p

        for level in range(lev[v], 1, -1):
            for u in by_level[level]:
                if (not alive[u]) or buf[u] == 0:
                    continue

                ptr = in_ptr[u]
                adj = in_edges[u]
                while ptr < len(adj) and buf[u] > 0:
                    eid = adj[ptr]
                    res = cap[eid] - flow[eid]
                    w = frm[eid]
                    if res <= 0 or (not alive[w]):
                        ptr += 1
                        continue

                    f = buf[u] if buf[u] < res else res
                    flow[eid] += f
                    buf[u] -= f
                    buf[w] += f
                    out_sum[w] -= f
                    in_sum[u] -= f

                    if flow[eid] == cap[eid]:
                        ptr += 1
                in_ptr[u] = ptr

        # kill any nodes that became zero-potential
        for i in range(1, n + 1):
            try_kill(i)
        kill_dead()

    # Output per input order (edge id is input order already)
    out = "\n".join(str(flow[i]) for i in range(m))
    sys.stdout.write(out)

if __name__ == "__main__":
    solve()
