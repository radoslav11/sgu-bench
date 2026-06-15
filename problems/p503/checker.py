"""Checker for p503: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal time is deterministic, so the value the solution prints must
equal the reference's optimal time. Reachability (the -1 case) must match
too. When a path exists, ANY optimal path is accepted: we re-validate the
printed edge sequence against the input (it must run from S to T with
consecutive edges sharing endpoints), recompute its total cost (base edge
weights plus the doubling penalty for every special route occurring as a
contiguous subpath, counted with multiplicity), and require that cost to
equal the reference's optimal time.
"""

import sys


def reject(msg):
    print(msg)
    sys.exit(1)


def main():
    in_tok = open(sys.argv[1]).read().split()
    out_tok = open(sys.argv[2]).read().split()
    ans_tok = open(sys.argv[3]).read().split()

    it = iter(in_tok)
    n = int(next(it))
    m = int(next(it))
    r = int(next(it))
    S = int(next(it))
    T = int(next(it))

    edges = []  # (a, b, c)
    for _ in range(m):
        a = int(next(it))
        b = int(next(it))
        c = int(next(it))
        edges.append((a, b, c))

    routes = []
    for _ in range(r):
        k = int(next(it))
        seq = [int(next(it)) - 1 for _ in range(k)]
        routes.append(seq)

    if not ans_tok:
        reject("empty reference answer")

    ref_neg = ans_tok[0] == "-1"
    if not out_tok:
        reject("empty output")
    out_neg = out_tok[0] == "-1"

    if ref_neg != out_neg:
        if ref_neg:
            reject("reference says T unreachable, output gave a path")
        else:
            reject("a path exists, output printed -1")

    if ref_neg:
        if len(out_tok) > 1:
            reject("trailing output after -1")
        sys.exit(0)

    ref_cost = int(ans_tok[0])

    # parse output: cost, count, edge list
    try:
        claimed = int(out_tok[0])
    except (ValueError, IndexError):
        reject("first output token is not an integer time")
    if len(out_tok) < 2:
        reject("missing edge count")
    try:
        cnt = int(out_tok[1])
    except ValueError:
        reject("edge count is not an integer")

    path = out_tok[2:]
    if len(path) != cnt:
        reject(f"declared {cnt} edges, listed {len(path)}")
    try:
        path = [int(x) - 1 for x in path]
    except ValueError:
        reject("non-integer edge id in path")

    if any(e < 0 or e >= m for e in path):
        reject("edge id out of range")

    # validate the walk runs from S to T with consecutive shared endpoints
    if cnt == 0:
        reject("empty path but S != T")
    a0 = edges[path[0]][0]
    if a0 != S:
        reject(f"path starts at {a0}, expected {S}")
    cur = edges[path[0]][1]
    base = edges[path[0]][2]
    for idx in range(1, cnt):
        e = path[idx]
        if edges[e][0] != cur:
            reject(f"edge {idx + 1} starts at {edges[e][0]}, expected {cur}")
        cur = edges[e][1]
        base += edges[e][2]
    if cur != T:
        reject(f"path ends at {cur}, expected {T}")

    # penalties: each special route occurring as a contiguous subpath adds its
    # base cost again (counted per occurrence, routes may repeat)
    penalty = 0
    for seq in routes:
        k = len(seq)
        if k == 0 or k > cnt:
            continue
        rcost = sum(edges[e][2] for e in seq)
        for start in range(cnt - k + 1):
            if path[start:start + k] == seq:
                penalty += rcost

    total = base + penalty

    if claimed != ref_cost:
        reject(f"claimed time {claimed} != optimal {ref_cost}")
    if total != ref_cost:
        reject(f"path actually costs {total}, not the claimed/optimal {ref_cost}")

    sys.exit(0)


if __name__ == "__main__":
    main()
