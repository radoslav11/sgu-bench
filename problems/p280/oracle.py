"""Second oracle for p280: greedy tree covering, iterative post-order DFS.

dp[u] = distance from u down to the nearest center placed in its subtree
(K+1 at a leaf meaning "none"). A center is forced when dp[u] reaches
2K+1. Finally the root gets a center if dp[root] > K.
"""

import sys


def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n, k = int(next(it)), int(next(it))
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        u, v = int(next(it)), int(next(it))
        adj[u].append(v)
        adj[v].append(u)

    parent = [0] * (n + 1)
    order = [1]
    parent[1] = -1
    for u in order:
        for v in adj[u]:
            if v != parent[u]:
                parent[v] = u
                order.append(v)

    dp = [0] * (n + 1)
    centers = []
    for u in reversed(order):
        child_dp = [dp[v] for v in adj[u] if v != parent[u]]
        if not child_dp:
            dp[u] = k + 1
        else:
            mn, mx = min(child_dp), max(child_dp)
            if mn + mx + 2 <= 2 * k + 1:
                dp[u] = mn + 1
            else:
                dp[u] = mx + 1
        if dp[u] == 2 * k + 1:
            dp[u] = 0
            centers.append(u)

    if dp[1] > k:
        centers.append(1)
    centers.sort()
    out = [str(len(centers))]
    out += [str(c) for c in centers]
    sys.stdout.write("\n".join(out) + "\n")


main()
