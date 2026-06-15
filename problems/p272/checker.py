"""Checker for p272: usage checker.py <in> <out> <ans>; exit 0 = accept.

K (length of every path) is deterministic: it must equal the reference
K. P is NOT compared to the reference, because any non-extendable plan
is allowed. The checker validates each path against the input graph
(starts in A, ends in B, follows tunnels, exactly K edges), checks
global vertex-disjointness, and finally re-runs a BFS over the unused
vertices to confirm no further length-K path from an unused A lab to an
unused B lab exists.
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

    pos = 0
    n, m = int(in_tokens[0]), int(in_tokens[1])
    pos = 2
    adj = [set() for _ in range(n + 1)]
    for _ in range(m):
        u, v = int(in_tokens[pos]), int(in_tokens[pos + 1])
        pos += 2
        adj[u].add(v)
        adj[v].add(u)
    n1 = int(in_tokens[pos])
    pos += 1
    set_a = set(int(t) for t in in_tokens[pos:pos + n1])
    pos += n1
    n2 = int(in_tokens[pos])
    pos += 1
    set_b = set(int(t) for t in in_tokens[pos:pos + n2])

    ref_k = int(ans_tokens[1])

    if len(out_tokens) < 2:
        reject("missing P and K")
    try:
        p, k = int(out_tokens[0]), int(out_tokens[1])
    except ValueError:
        reject("P and K must be integers")
    if k != ref_k:
        reject(f"K must be {ref_k}, got {k}")
    if p < 0:
        reject("negative P")
    body = out_tokens[2:]
    if len(body) != p * (k + 1):
        reject(f"expected {p * (k + 1)} path tokens, got {len(body)}")

    used = set()
    for i in range(p):
        path = [int(t) for t in body[i * (k + 1):(i + 1) * (k + 1)]]
        if any(not (1 <= v <= n) for v in path):
            reject(f"path {i + 1}: vertex out of range")
        if path[0] not in set_a:
            reject(f"path {i + 1}: does not start in A")
        if path[-1] not in set_b:
            reject(f"path {i + 1}: does not end in B")
        for a, b in zip(path, path[1:]):
            if b not in adj[a]:
                reject(f"path {i + 1}: no tunnel between {a} and {b}")
        for v in path:
            if v in used:
                reject(f"laboratory {v} used by two paths")
            used.add(v)

    # extendability: BFS from free A labs over free vertices only
    free = [v not in used for v in range(n + 1)]
    dist = [-1] * (n + 1)
    queue = deque()
    for a in set_a:
        if free[a]:
            dist[a] = 0
            queue.append(a)
    while queue:
        v = queue.popleft()
        if dist[v] >= k:
            continue
        for to in adj[v]:
            if free[to] and dist[to] == -1:
                dist[to] = dist[v] + 1
                queue.append(to)
    for b in set_b:
        if free[b] and dist[b] == k:
            reject(f"plan is extendable: free path to exit {b} exists")

    sys.exit(0)


if __name__ == "__main__":
    main()
