"""Checker for p212: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any blocking flow is accepted, so the reference answer is not compared
at all. The output must give one flow value per channel within its
capacity, conserve flow at every node except the source and target, and
be blocking: following only non-saturated channels (flow < capacity)
there must be no path from the source to the target.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n, m, levels_count = (
        int(in_tokens[0]),
        int(in_tokens[1]),
        int(in_tokens[2]),
    )
    level = [0] + [int(x) for x in in_tokens[3 : 3 + n]]
    src = level.index(1)
    snk = level.index(levels_count)

    edges = []
    base = 3 + n
    for i in range(m):
        a = int(in_tokens[base + 3 * i])
        b = int(in_tokens[base + 3 * i + 1])
        c = int(in_tokens[base + 3 * i + 2])
        edges.append((a, b, c))

    if len(out_tokens) != m:
        reject(f"expected {m} flow values, got {len(out_tokens)}")
    flow = []
    for tok in out_tokens:
        try:
            flow.append(int(tok))
        except ValueError:
            reject(f"non-integer flow value '{tok}'")

    balance = [0] * (n + 1)
    adj = [[] for _ in range(n + 1)]
    for (a, b, c), f in zip(edges, flow):
        if f < 0 or f > c:
            reject(f"flow {f} outside [0, {c}] on channel {a}->{b}")
        balance[a] -= f
        balance[b] += f
        if f < c:
            adj[a].append(b)

    for v in range(1, n + 1):
        if v in (src, snk):
            continue
        if balance[v] != 0:
            reject(f"flow not conserved at node {v} (excess {balance[v]})")
    if balance[snk] != -balance[src]:
        reject("source outflow differs from target inflow")
    if balance[snk] < 0:
        reject("negative flow value from source to target")

    # blocking check: no source->target path over non-saturated channels
    seen = [False] * (n + 1)
    seen[src] = True
    queue = deque([src])
    while queue:
        v = queue.popleft()
        if v == snk:
            reject("flow is not blocking: augmenting path of "
                   "non-saturated channels exists")
        for u in adj[v]:
            if not seen[u]:
                seen[u] = True
                queue.append(u)

    sys.exit(0)


if __name__ == "__main__":
    main()
