"""Checker for p424: usage checker.py <in> <out> <ans>; exit 0 = accept.

The edge count E is deterministic (the maximum over all beautiful graphs
on at most n vertices), so it must match the reference's E. The graph
itself is a certificate: it must be a simple loopless graph on V <= n
vertices that is "beautiful", i.e. it contains no two simple cycles
sharing exactly one edge. Two such cycles through an edge e = (u, v)
exist iff after removing e there are two edge-disjoint u-v paths, so
beauty is verified with a tiny unit-capacity max-flow per edge.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def two_edge_disjoint_paths(adj_edges, n_vertices, s, t, banned):
    # Edge-capacity-1 max flow via BFS augmentation, stop at flow 2.
    # adj_edges: list of (u, v); residual kept as per-edge capacities
    # in both directions, with the banned edge index removed.
    cap = {}
    graph = [[] for _ in range(n_vertices + 1)]
    for idx, (u, v) in enumerate(adj_edges):
        if idx == banned:
            continue
        graph[u].append(v)
        graph[v].append(u)
        cap[(u, v)] = cap.get((u, v), 0) + 1
        cap[(v, u)] = cap.get((v, u), 0) + 1

    flow = 0
    while flow < 2:
        parent = [0] * (n_vertices + 1)
        seen = [False] * (n_vertices + 1)
        seen[s] = True
        queue = deque([s])
        while queue:
            x = queue.popleft()
            if x == t:
                break
            for y in graph[x]:
                if not seen[y] and cap.get((x, y), 0) > 0:
                    seen[y] = True
                    parent[y] = x
                    queue.append(y)
        if not seen[t]:
            break
        cur = t
        while cur != s:
            p = parent[cur]
            cap[(p, cur)] -= 1
            cap[(cur, p)] = cap.get((cur, p), 0) + 1
            cur = p
        flow += 1

    return flow >= 2


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    expected_e = int(ans_tokens[1])

    if len(out_tokens) < 2:
        reject("missing V and E")
    try:
        v_cnt = int(out_tokens[0])
        e_cnt = int(out_tokens[1])
    except ValueError:
        reject("V and E must be integers")
    if not (1 <= v_cnt <= n):
        reject(f"V={v_cnt} out of range [1, {n}]")
    if e_cnt != expected_e:
        reject(f"edge count {e_cnt} is not maximal ({expected_e})")
    if len(out_tokens) != 2 + 2 * e_cnt:
        reject(f"expected {2 * e_cnt} edge tokens, got {len(out_tokens) - 2}")

    edges = []
    seen_pairs = set()
    for i in range(e_cnt):
        try:
            u = int(out_tokens[2 + 2 * i])
            v = int(out_tokens[3 + 2 * i])
        except ValueError:
            reject(f"edge {i + 1}: non-integer endpoint")
        if not (1 <= u <= v_cnt and 1 <= v <= v_cnt):
            reject(f"edge {i + 1}: endpoint out of range")
        if u == v:
            reject(f"edge {i + 1}: loop at vertex {u}")
        key = (min(u, v), max(u, v))
        if key in seen_pairs:
            reject(f"edge {i + 1}: duplicate edge {key}")
        seen_pairs.add(key)
        edges.append((u, v))

    for idx, (u, v) in enumerate(edges):
        if two_edge_disjoint_paths(edges, v_cnt, u, v, idx):
            reject(
                f"two simple cycles share exactly edge ({u}, {v})"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
