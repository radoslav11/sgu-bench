"""Checker for p303: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum wall cost is deterministic and must match the reference's
first number. The segment list is a certificate: indices must be valid
and distinct, the chosen segments must form one simple closed polygon
(every endpoint has degree 2 and the edge set is connected; the input
guarantees segments share at most endpoints), the costs must sum to the
claimed minimum, and exactly one headquarters must lie strictly inside.
"""

import sys
from fractions import Fraction


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tok = open(sys.argv[1]).read().split()
    out_tok = open(sys.argv[2]).read().split()
    ans_tok = open(sys.argv[3]).read().split()

    n = int(in_tok[0])
    segs = []
    for i in range(n):
        x1, y1, x2, y2, v = (int(t) for t in in_tok[1 + 5 * i: 6 + 5 * i])
        segs.append(((x1, y1), (x2, y2), v))
    hx1, hy1, hx2, hy2 = (int(t) for t in in_tok[1 + 5 * n: 5 + 5 * n])

    expected_cost = int(ans_tok[0])

    if len(out_tok) < 2:
        reject("output too short")
    try:
        cost = int(out_tok[0])
        k = int(out_tok[1])
        idx = [int(t) for t in out_tok[2:]]
    except ValueError:
        reject("non-integer token in output")
    if cost != expected_cost:
        reject(f"cost {cost} != optimal {expected_cost}")
    if len(idx) != k:
        reject(f"declared {k} segments, listed {len(idx)}")
    if k < 3:
        reject("a simple polygon needs at least 3 segments")
    if len(set(idx)) != k:
        reject("duplicate segment indices")
    if any(not (1 <= i <= n) for i in idx):
        reject("segment index out of range")

    chosen = [segs[i - 1] for i in idx]
    if sum(v for _, _, v in chosen) != cost:
        reject("segment costs do not sum to the claimed cost")

    # Simple closed polygon: every endpoint must have degree exactly 2
    # and the chosen segments must be connected. Segments in the input
    # only meet at endpoints, so this implies a simple polygon.
    deg = {}
    adj = {}
    for a, b, _ in chosen:
        if a == b:
            reject("degenerate zero-length segment chosen")
        for p, q in ((a, b), (b, a)):
            deg[p] = deg.get(p, 0) + 1
            adj.setdefault(p, []).append(q)
    for p, d in deg.items():
        if d != 2:
            reject(f"vertex {p} has degree {d}, expected 2")
    start = chosen[0][0]
    seen = {start}
    stack = [start]
    while stack:
        u = stack.pop()
        for v in adj[u]:
            if v not in seen:
                seen.add(v)
                stack.append(v)
    if len(seen) != len(deg):
        reject("chosen segments form more than one cycle")

    def inside(px, py):
        # Upward-ray crossing count with the half-open rule [x1, x2).
        cnt = 0
        for (x1, y1), (x2, y2), _ in chosen:
            if x1 == x2:
                continue
            if (x1 <= px < x2) or (x2 <= px < x1):
                ycross = Fraction(y1) + Fraction(y2 - y1, x2 - x1) * (px - x1)
                if ycross > py:
                    cnt += 1
        return cnt % 2 == 1

    in1 = inside(hx1, hy1)
    in2 = inside(hx2, hy2)
    if in1 == in2:
        reject(
            f"wall does not separate the headquarters "
            f"(inside flags {in1}, {in2})"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
