"""Checker for p206: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any d vector is accepted that (a) reaches the same total adjustment
sum |c_i - d_i| as the reference and (b) makes roads 1..N-1 a valid
minimum spanning tree choice under d: for every non-tree road, every
tree road on the cycle it closes has d at most the non-tree road's d.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    edges = []
    for i in range(m):
        a = int(in_tokens[2 + 3 * i])
        b = int(in_tokens[3 + 3 * i])
        c = int(in_tokens[4 + 3 * i])
        edges.append((a, b, c))

    if len(out_tokens) != m:
        reject(f"expected {m} numbers, got {len(out_tokens)}")
    try:
        d = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    ref_d = [int(t) for t in ans_tokens]
    ref_sum = sum(abs(edges[i][2] - ref_d[i]) for i in range(m))
    got_sum = sum(abs(edges[i][2] - d[i]) for i in range(m))
    if got_sum != ref_sum:
        reject(f"total adjustment {got_sum} != optimal {ref_sum}")

    adj = [[] for _ in range(n + 1)]
    for i in range(n - 1):
        a, b, _ = edges[i]
        adj[a].append((b, i))
        adj[b].append((a, i))

    parent = [0] * (n + 1)
    parent_edge = [-1] * (n + 1)
    depth = [0] * (n + 1)
    seen = [False] * (n + 1)
    queue = deque([1])
    seen[1] = True
    while queue:
        u = queue.popleft()
        for v, idx in adj[u]:
            if not seen[v]:
                seen[v] = True
                parent[v] = u
                parent_edge[v] = idx
                depth[v] = depth[u] + 1
                queue.append(v)

    for j in range(n - 1, m):
        a, b, _ = edges[j]
        path_max = -(10**18)
        u, v = a, b
        while u != v:
            if depth[u] < depth[v]:
                u, v = v, u
            path_max = max(path_max, d[parent_edge[u]])
            u = parent[u]
        if path_max > d[j]:
            reject(
                f"road {j + 1}: tree path has d={path_max} > d={d[j]}, "
                "wizard would drop a stoned road"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
