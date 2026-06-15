"""Checker for p321: usage checker.py <in> <out> <ans>; exit 0 = accept.

The number of converted channels is deterministic and must match the
reference. The channel list is a certificate: distinct channels that are
almost protected in the input, and after converting them every node's
path to Kat (node 1) must have at least half of its channels protected.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tok = open(sys.argv[1]).read().split()
    out_tok = open(sys.argv[2]).read().split()
    ans_tok = open(sys.argv[3]).read().split()

    n = int(in_tok[0])
    pos = 1
    # adj[u] = (v, channel index, protected flag)
    adj = [[] for _ in range(n + 1)]
    is_almost = [False] * n
    for idx in range(1, n):
        u = int(in_tok[pos])
        v = int(in_tok[pos + 1])
        pos += 2
        if in_tok[pos] == "almost":
            pos += 2
            is_almost[idx] = True
            adj[u].append((v, idx))
            adj[v].append((u, idx))
        else:
            pos += 1
            adj[u].append((v, 0))
            adj[v].append((u, 0))

    expected = int(ans_tok[0])
    if not out_tok:
        reject("empty output")
    try:
        cnt = int(out_tok[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tok[0]}'")
    if cnt != expected:
        reject(f"optimal count is {expected}, got {cnt}")

    rest = out_tok[1:]
    if len(rest) != cnt:
        reject(f"declared {cnt} channels, listed {len(rest)}")
    try:
        chosen = [int(x) for x in rest]
    except ValueError:
        reject("non-integer channel index")
    if len(set(chosen)) != cnt:
        reject("duplicate channel indices")
    for c in chosen:
        if not (1 <= c <= n - 1):
            reject(f"channel index {c} out of range")
        if not is_almost[c]:
            reject(f"channel {c} is already protected")

    converted = set(chosen)

    # BFS from Kat; state per node: path length, protected count
    depth = [0] * (n + 1)
    prot = [0] * (n + 1)
    seen = [False] * (n + 1)
    seen[1] = True
    q = deque([1])
    while q:
        u = q.popleft()
        if 2 * prot[u] < depth[u]:
            reject(f"node {u}: only {prot[u]} of {depth[u]} channels protected")
        for v, idx in adj[u]:
            if not seen[v]:
                seen[v] = True
                depth[v] = depth[u] + 1
                good = idx == 0 or idx in converted
                prot[v] = prot[u] + (1 if good else 0)
                q.append(v)

    sys.exit(0)


if __name__ == "__main__":
    main()
