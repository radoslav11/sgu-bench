"""Checker for p402: usage checker.py <in> <out> <ans>; exit 0 = accept.

The total destruction cost is deterministic and must match the
reference. The road list is a certificate: distinct road indices whose
costs sum to the claimed total, such that some city c exists where the
graph minus the destroyed roads and minus c falls apart into at least
two components. Any such plan is accepted; the reference's own list is
never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    edges = []
    for i in range(m):
        a = int(in_tokens[2 + 3 * i])
        b = int(in_tokens[3 + 3 * i])
        w = int(in_tokens[4 + 3 * i])
        edges.append((a, b, w))

    cost_ref = int(ans_tokens[0])
    if len(out_tokens) < 2:
        reject("output must contain cost and road count")
    try:
        cost_out = int(out_tokens[0])
        k = int(out_tokens[1])
    except ValueError:
        reject("cost/count are not integers")
    if cost_out != cost_ref:
        reject(f"optimal cost is {cost_ref}, got {cost_out}")
    if len(out_tokens) != 2 + k:
        reject(f"declared {k} roads, listed {len(out_tokens) - 2}")
    try:
        destroyed = [int(x) for x in out_tokens[2:]]
    except ValueError:
        reject("road indices are not integers")
    if len(set(destroyed)) != k:
        reject("duplicate road indices")
    if any(not (1 <= idx <= m) for idx in destroyed):
        reject("road index out of range")
    if sum(edges[idx - 1][2] for idx in destroyed) != cost_out:
        reject("destroyed roads' costs do not sum to the claimed total")

    gone = set(destroyed)
    kept = [(a, b) for i, (a, b, _) in enumerate(edges, 1) if i not in gone]

    for captured in range(1, n + 1):
        parent = list(range(n + 1))

        def find(x):
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        for a, b in kept:
            if a == captured or b == captured:
                continue
            parent[find(a)] = find(b)
        roots = {find(v) for v in range(1, n + 1) if v != captured}
        if len(roots) >= 2:
            sys.exit(0)

    reject("no captured city divides the country after these destructions")


if __name__ == "__main__":
    main()
