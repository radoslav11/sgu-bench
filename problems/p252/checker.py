"""Checker for p252: usage checker.py <in> <out> <ans>; exit 0 = accept.

K (minimal number of paths) and C (minimal maintenance cost among
K-path schedules) are unique and must match the reference's first
line. The schedule itself may be any partition of the towns into K
railroad paths whose used edges cost exactly C; it is validated
against the input graph, never against the reference's own paths.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_raw = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    cost = {}
    for i in range(m):
        a = int(in_tokens[2 + 3 * i])
        b = int(in_tokens[3 + 3 * i])
        c = int(in_tokens[4 + 3 * i])
        cost[(a, b)] = c

    ref_k, ref_c = int(ans_tokens[0]), int(ans_tokens[1])

    try:
        out = [int(t) for t in out_raw]
    except ValueError:
        reject("output contains non-integer tokens")
    if len(out) < 2:
        reject("missing K and C")
    k, c = out[0], out[1]
    if k != ref_k:
        reject(f"minimal number of paths is {ref_k}, got {k}")
    if c != ref_c:
        reject(f"minimal cost is {ref_c}, got {c}")

    pos = 2
    visited = set()
    total_cost = 0
    for p in range(k):
        if pos >= len(out):
            reject(f"only {p} of {k} paths present")
        length = out[pos]
        pos += 1
        if not (1 <= length <= n):
            reject(f"path {p + 1} has invalid length {length}")
        if pos + length > len(out):
            reject(f"path {p + 1} is truncated")
        towns = out[pos:pos + length]
        pos += length
        for t in towns:
            if not (1 <= t <= n):
                reject(f"town {t} out of range 1..{n}")
            if t in visited:
                reject(f"town {t} visited twice")
            visited.add(t)
        for u, v in zip(towns, towns[1:]):
            if (u, v) not in cost:
                reject(f"no railroad from {u} to {v}")
            total_cost += cost[(u, v)]

    if pos != len(out):
        reject("trailing output after the last path")
    if len(visited) != n:
        reject(f"only {len(visited)} of {n} towns are covered")
    if total_cost != c:
        reject(f"paths use railroads costing {total_cost}, claimed {c}")

    sys.exit(0)


if __name__ == "__main__":
    main()
