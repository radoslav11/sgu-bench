"""Tests for p122 (The book): 2 <= N <= 1000, friendship symmetric, and
every man has at least floor((N+1)/2) friends (Dirac's condition).

Random graphs are repaired to meet the degree bound by linking deficient
vertices to random others. Seed 1 is N = 2, seed 2 the complete graph
on 1000, seed 3 the tight bipartite K(500,500), seed 4 the complete
graph on 1000 minus a perfect matching, the rest random graphs just
above the threshold, odd and even N. (Sparser structured cases such as
overlapping cliques break the trusted reference, so they are avoided.)
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(n, adj):
    print(n)
    out = []
    for u in range(1, n + 1):
        lst = sorted(adj[u])
        rng.shuffle(lst)
        out.append(" ".join(map(str, lst)))
    print("\n".join(out))


def repair(n, adj):
    need = (n + 1) // 2
    for u in range(1, n + 1):
        others = [v for v in range(1, n + 1) if v != u and v not in adj[u]]
        rng.shuffle(others)
        while len(adj[u]) < need and others:
            v = others.pop()
            adj[u].add(v)
            adj[v].add(u)
    return adj


if seed == 1:
    emit(2, {1: {2}, 2: {1}})
elif seed == 2:
    n = 1000
    emit(n, {u: set(range(1, n + 1)) - {u} for u in range(1, n + 1)})
elif seed == 3:
    n = 1000
    left = set(range(1, 501))
    right = set(range(501, 1001))
    adj = {u: (right if u in left else left) for u in range(1, n + 1)}
    emit(n, adj)
elif seed == 4:
    n = 1000
    adj = {}
    for u in range(1, n + 1):
        mate = u + 1 if u % 2 == 1 else u - 1
        adj[u] = set(range(1, n + 1)) - {u, mate}
    emit(n, adj)
else:
    n = rng.choice([rng.randint(3, 60), rng.randint(500, 1000)])
    p = rng.uniform(0.5, 0.7)
    adj = {u: set() for u in range(1, n + 1)}
    for u in range(1, n + 1):
        for v in range(u + 1, n + 1):
            if rng.random() < p:
                adj[u].add(v)
                adj[v].add(u)
    emit(n, repair(n, adj))
