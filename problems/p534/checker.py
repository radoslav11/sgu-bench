"""Checker for p534: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum cost is deterministic and must equal the reference cost in
<ans>. The replaced-edge set is a certificate: any set is accepted as long
as (1) the edges are valid, distinct, 1-based indices, (2) their total
replacement cost equals the printed cost, and (3) zeroing those edges makes
the tree diameter strictly less than the original diameter D. The
reference's own edge set is never compared against.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def read_tree(in_path):
    tok = open(in_path).read().split()
    it = iter(tok)
    n = int(next(it))
    adj = [[] for _ in range(n + 1)]
    edges = []  # (a, b, t, p)
    for i in range(n - 1):
        a = int(next(it))
        b = int(next(it))
        t = int(next(it))
        p = int(next(it))
        adj[a].append((b, t))
        adj[b].append((a, t))
        edges.append((a, b, t, p))
    return n, adj, edges


def diameter(n, adj_weighted):
    # adj_weighted[u] = list of (v, weight); compute weighted diameter via two BFS.
    def bfs(start):
        dist = [-1] * (n + 1)
        dist[start] = 0
        q = deque([start])
        while q:
            u = q.popleft()
            for v, w in adj_weighted[u]:
                if dist[v] == -1:
                    dist[v] = dist[u] + w
                    q.append(v)
        return dist

    d1 = bfs(1)
    u = max(range(1, n + 1), key=lambda i: d1[i])
    du = bfs(u)
    far = max(range(1, n + 1), key=lambda i: du[i])
    return du[far]


def main():
    n, adj, edges = read_tree(sys.argv[1])

    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    if not ans_tokens:
        reject("empty reference answer")
    ref_cost = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        cost = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the cost, got '{out_tokens[0]}'")

    if cost != ref_cost:
        reject(f"cost is {ref_cost}, got {cost}")

    if len(out_tokens) < 2:
        reject("missing count of replaced edges")
    try:
        k = int(out_tokens[1])
    except ValueError:
        reject(f"count must be an integer, got '{out_tokens[1]}'")

    idx_tokens = out_tokens[2:]
    if len(idx_tokens) != k:
        reject(f"declared {k} edges, listed {len(idx_tokens)}")

    chosen = []
    for tk in idx_tokens:
        try:
            e = int(tk)
        except ValueError:
            reject(f"edge index not an integer: '{tk}'")
        if not (1 <= e <= n - 1):
            reject(f"edge index out of range: {e}")
        chosen.append(e)

    if len(set(chosen)) != len(chosen):
        reject("duplicate edge indices in replaced set")

    total = sum(edges[e - 1][3] for e in chosen)
    if total != cost:
        reject(f"listed edges total cost {total}, but printed cost {cost}")

    # Original diameter.
    orig_adj = [[] for _ in range(n + 1)]
    for a, b, t, p in edges:
        orig_adj[a].append((b, t))
        orig_adj[b].append((a, t))
    D = diameter(n, orig_adj)

    # Diameter after zeroing chosen edges.
    replaced = set(chosen)
    new_adj = [[] for _ in range(n + 1)]
    for i, (a, b, t, p) in enumerate(edges):
        w = 0 if (i + 1) in replaced else t
        new_adj[a].append((b, w))
        new_adj[b].append((a, w))
    new_D = diameter(n, new_adj)

    if not (new_D < D):
        reject(f"new diameter {new_D} is not less than original {D}")

    sys.exit(0)


if __name__ == "__main__":
    main()
