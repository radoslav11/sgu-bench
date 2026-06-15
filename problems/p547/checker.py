"""Checker for p547: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum destruction cost (or -1 when impossible) is deterministic and
must equal the reference value. When a solution exists the output's second
line lists the destroyed cities; any list is accepted as long as it costs
exactly the reference cost and the surviving cities split into a valid
King part and a valid Queen part:

  - removing the destroyed cities leaves exactly two connected components,
  - one component has tree-diameter D1 with at most C1 distinct cities
    appearing in diameter-achieving pairs, the other has D2 / C2
    (either assignment of the two components is allowed),
  - both components are non-empty.

The reference's own destroyed set is never required to match.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def read_input(path):
    tok = open(path).read().split()
    it = iter(tok)
    n = int(next(it))
    d1, c1, d2, c2 = (int(next(it)) for _ in range(4))
    p = [int(next(it)) for _ in range(n)]
    adj = [[] for _ in range(n + 1)]
    edges = []
    for _ in range(n - 1):
        a = int(next(it))
        b = int(next(it))
        adj[a].append(b)
        adj[b].append(a)
        edges.append((a, b))
    return n, d1, c1, d2, c2, p, adj, edges


def components(alive, adj):
    seen = set()
    comps = []
    for s in alive:
        if s in seen:
            continue
        comp = []
        seen.add(s)
        q = deque([s])
        while q:
            u = q.popleft()
            comp.append(u)
            for v in adj[u]:
                if v in alive and v not in seen:
                    seen.add(v)
                    q.append(v)
        comps.append(comp)
    return comps


def bfs_dist(src, nodes_set, adj):
    dist = {src: 0}
    q = deque([src])
    while q:
        u = q.popleft()
        for v in adj[u]:
            if v in nodes_set and v not in dist:
                dist[v] = dist[u] + 1
                q.append(v)
    return dist


def diameter_and_endpoint_count(comp, adj):
    nodes_set = set(comp)
    D = 0
    all_dist = {}
    for u in comp:
        d = bfs_dist(u, nodes_set, adj)
        all_dist[u] = d
        m = max(d.values())
        if m > D:
            D = m
    endpoints = set()
    for u in comp:
        for v in comp:
            if u < v and all_dist[u][v] == D:
                endpoints.add(u)
                endpoints.add(v)
    return D, len(endpoints)


def comp_ok(comp, D, C, adj):
    diam, ecount = diameter_and_endpoint_count(comp, adj)
    return diam == D and ecount <= C


def main():
    n, d1, c1, d2, c2, p, adj, edges = read_input(sys.argv[1])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    if not ans_tokens:
        reject("reference produced no answer")
    ref_first = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        got_cost = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")

    if ref_first == -1:
        if got_cost != -1:
            reject(f"reference says impossible (-1), got {got_cost}")
        sys.exit(0)

    if got_cost == -1:
        reject(f"answer is feasible (cost {ref_first}), got -1")
    if got_cost != ref_first:
        reject(f"min cost is {ref_first}, got {got_cost}")

    destroyed = [int(x) for x in out_tokens[1:]]
    if len(set(destroyed)) != len(destroyed):
        reject("duplicate city in destroyed list")
    if any(not (1 <= c <= n) for c in destroyed):
        reject("destroyed city out of range")

    dset = set(destroyed)
    cost = sum(p[c - 1] for c in dset)
    if cost != got_cost:
        reject(f"destroyed cities cost {cost}, declared {got_cost}")

    alive = [v for v in range(1, n + 1) if v not in dset]
    if not alive:
        reject("no surviving cities")

    comps = components(alive, adj)
    if len(comps) != 2:
        reject(f"surviving cities form {len(comps)} components, expected 2")

    a, b = comps
    if (comp_ok(a, d1, c1, adj) and comp_ok(b, d2, c2, adj)) or (
        comp_ok(a, d2, c2, adj) and comp_ok(b, d1, c1, adj)
    ):
        sys.exit(0)

    da, ea = diameter_and_endpoint_count(a, adj)
    db, eb = diameter_and_endpoint_count(b, adj)
    reject(
        f"components do not satisfy (D1,C1)/(D2,C2): "
        f"comp A diam={da} endpts={ea}, comp B diam={db} endpts={eb}; "
        f"need D1={d1} C1={c1}, D2={d2} C2={c2}"
    )


if __name__ == "__main__":
    main()
