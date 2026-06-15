"""Checker for p322: usage checker.py <in> <out> <ans>; exit 0 = accept.

The number of months Q is deterministic and must match the reference.
The plan is a certificate: each month adds a non-existing road to the
chosen country's tree and closes a road on the resulting cycle (the
network must stay a tree), and afterwards both edge sets must be equal.
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
    trees = [set(), set()]
    for t in range(2):
        for _ in range(n - 1):
            u, v = int(in_tok[pos]), int(in_tok[pos + 1])
            pos += 2
            trees[t].add(frozenset((u, v)))

    expected = int(ans_tok[0])
    if not out_tok:
        reject("empty output")
    try:
        q = int(out_tok[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tok[0]}'")
    if q != expected:
        reject(f"optimal number of months is {expected}, got {q}")
    if len(out_tok) != 1 + 5 * q:
        reject(f"expected {5 * q} numbers after Q, got {len(out_tok) - 1}")

    def connected(edge_set):
        adj = [[] for _ in range(n + 1)]
        for e in edge_set:
            u, v = tuple(e)
            adj[u].append(v)
            adj[v].append(u)
        seen = [False] * (n + 1)
        seen[1] = True
        dq = deque([1])
        cnt = 1
        while dq:
            x = dq.popleft()
            for y in adj[x]:
                if not seen[y]:
                    seen[y] = True
                    cnt += 1
                    dq.append(y)
        return cnt == n

    for i in range(q):
        try:
            p, a1, b1, a2, b2 = (int(x) for x in out_tok[1 + 5 * i:6 + 5 * i])
        except ValueError:
            reject(f"month {i + 1}: non-integer values")
        if p not in (1, 2):
            reject(f"month {i + 1}: country must be 1 or 2, got {p}")
        for x in (a1, b1, a2, b2):
            if not (1 <= x <= n):
                reject(f"month {i + 1}: city {x} out of range")
        if a1 == b1 or a2 == b2:
            reject(f"month {i + 1}: road endpoints coincide")
        tree = trees[p - 1]
        add = frozenset((a1, b1))
        rem = frozenset((a2, b2))
        if add in tree:
            reject(f"month {i + 1}: road {a1}-{b1} already exists")
        tree.add(add)
        if rem not in tree:
            reject(f"month {i + 1}: road {a2}-{b2} does not exist")
        tree.remove(rem)
        if not connected(tree):
            reject(f"month {i + 1}: closing {a2}-{b2} disconnects the network")

    if trees[0] != trees[1]:
        reject("road networks are not identical after the plan")

    sys.exit(0)


if __name__ == "__main__":
    main()
