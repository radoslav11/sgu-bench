"""Checker for p216: usage checker.py <in> <out> <ans>; exit 0 = accept.

A plan always exists for 1 <= B <= N (the reference always prints one),
so an output of 0 is rejected. Any partition of the cities into K
provinces of size B..3B is accepted, provided each province together
with its capital city forms a connected piece of the tree (then every
citizen reaches the capital through his own province only). The plan is
never compared against the reference's own partition.
"""

import sys
from collections import deque


def reject(message):
    print(message)
    sys.exit(1)


def read_ints(path):
    try:
        return [int(t) for t in open(path).read().split()]
    except ValueError:
        return None


def main():
    in_tokens = read_ints(sys.argv[1])
    out_tokens = read_ints(sys.argv[2])
    if out_tokens is None:
        reject("output contains non-integer tokens")

    n, b = in_tokens[0], in_tokens[1]
    adj = [[] for _ in range(n + 1)]
    for i in range(n - 1):
        u, v = in_tokens[2 + 2 * i], in_tokens[3 + 2 * i]
        adj[u].append(v)
        adj[v].append(u)

    if not out_tokens:
        reject("empty output")
    k = out_tokens[0]
    if k == 0:
        reject("claimed impossible, but a valid plan exists")
    if k < 0:
        reject(f"invalid province count {k}")
    if len(out_tokens) != 1 + n + k:
        reject(
            f"expected {1 + n + k} tokens (K, N provinces, K capitals), "
            f"got {len(out_tokens)}"
        )

    prov = out_tokens[1:1 + n]
    capitals = out_tokens[1 + n:]
    if any(not (1 <= p <= k) for p in prov):
        reject("province number out of range 1..K")
    if any(not (1 <= c <= n) for c in capitals):
        reject("capital city out of range 1..N")

    members = [[] for _ in range(k + 1)]
    for city, p in enumerate(prov, start=1):
        members[p].append(city)

    for p in range(1, k + 1):
        size = len(members[p])
        if not (b <= size <= 3 * b):
            reject(f"province {p} has {size} cities, outside [{b}, {3 * b}]")
        cap = capitals[p - 1]
        # BFS over the province's cities plus the capital itself.
        seen = {cap}
        queue = deque([cap])
        reached = 1 if prov[cap - 1] == p else 0
        while queue:
            u = queue.popleft()
            if u != cap and prov[u - 1] != p:
                continue
            for v in adj[u]:
                if v not in seen and (v == cap or prov[v - 1] == p):
                    seen.add(v)
                    reached += 1 if prov[v - 1] == p else 0
                    queue.append(v)
        if reached != size:
            reject(
                f"province {p} cannot fully reach capital {cap} "
                f"through its own cities"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
