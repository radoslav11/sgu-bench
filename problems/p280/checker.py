"""Checker for p280: usage checker.py <in> <out> <ans>; exit 0 = accept.

The number of centers is the optimal value and must match the reference.
The list of cities is a certificate: it is validated against the input
tree with a multi-source BFS — every city must be within K roads of some
listed center. Any optimal placement is accepted.
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

    n, k = int(in_tokens[0]), int(in_tokens[1])
    adj = [[] for _ in range(n + 1)]
    pos = 2
    for _ in range(n - 1):
        u, v = int(in_tokens[pos]), int(in_tokens[pos + 1])
        pos += 2
        adj[u].append(v)
        adj[v].append(u)

    expected = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        m = int(out_tokens[0])
        centers = [int(x) for x in out_tokens[1:]]
    except ValueError:
        reject("non-integer token in output")
    if m != expected:
        reject(f"optimal center count is {expected}, got {m}")
    if len(centers) != m:
        reject(f"declared {m} centers, listed {len(centers)}")
    if len(set(centers)) != m:
        reject("duplicate centers")
    if any(not (1 <= c <= n) for c in centers):
        reject("center out of range")

    dist = [-1] * (n + 1)
    queue = deque()
    for c in centers:
        dist[c] = 0
        queue.append(c)
    while queue:
        u = queue.popleft()
        if dist[u] == k:
            continue
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                queue.append(v)

    for v in range(1, n + 1):
        if dist[v] == -1:
            reject(f"city {v} is farther than {k} roads from every center")

    sys.exit(0)


if __name__ == "__main__":
    main()
