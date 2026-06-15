"""Checker for p213: usage checker.py <in> <out> <ans>; exit 0 = accept.

The number of crystal types L is deterministic (the S-T BFS distance)
and must match the reference. The groups are a certificate: edge indices
must be valid and pairwise disjoint across all groups, and removing any
single group's edges must disconnect S from T. Any such family of L
disjoint cuts is accepted.
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

    n, m, s, t = (int(x) for x in in_tokens[:4])
    edges = []
    for i in range(m):
        a = int(in_tokens[4 + 2 * i])
        b = int(in_tokens[5 + 2 * i])
        edges.append((a, b))

    expected_l = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        l = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if l != expected_l:
        reject(f"maximum number of crystal types is {expected_l}, got {l}")

    pos = 1
    groups = []
    used = set()
    for g in range(l):
        if pos >= len(out_tokens):
            reject(f"output ended before group {g + 1}")
        try:
            k = int(out_tokens[pos])
        except ValueError:
            reject(f"bad group size '{out_tokens[pos]}'")
        pos += 1
        if k < 1:
            reject(f"group {g + 1} is empty")
        if pos + k > len(out_tokens):
            reject(f"output ended inside group {g + 1}")
        ids = []
        for tok in out_tokens[pos : pos + k]:
            try:
                ids.append(int(tok))
            except ValueError:
                reject(f"bad edge index '{tok}'")
        pos += k
        for e in ids:
            if not (1 <= e <= m):
                reject(f"edge index {e} out of range")
            if e in used:
                reject(f"edge {e} used in more than one group")
            used.add(e)
        groups.append(ids)
    if pos != len(out_tokens):
        reject("trailing output after the last group")

    for g, ids in enumerate(groups):
        blocked = set(ids)
        adj = [[] for _ in range(n + 1)]
        for idx, (a, b) in enumerate(edges, start=1):
            if idx not in blocked:
                adj[a].append(b)
                adj[b].append(a)
        seen = [False] * (n + 1)
        seen[s] = True
        queue = deque([s])
        while queue:
            v = queue.popleft()
            for u in adj[v]:
                if not seen[u]:
                    seen[u] = True
                    queue.append(u)
        if seen[t]:
            reject(f"group {g + 1} does not disconnect S from T")

    sys.exit(0)


if __name__ == "__main__":
    main()
