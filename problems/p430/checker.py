"""Checker for p430 (Unit-distance graph): checker.py <in> <out> <ans>.

The decision "Yes"/"No" is unique, but the reference is a randomized
gradient-descent search, so its "No" is not a hard proof. We therefore
trust certificates over the reference verdict:

- A "Yes" output must carry a valid embedding: n points with |coord| <= 100,
  every edge pair at distance within 1e-7 of 1, every non-edge pair farther
  than 1e-2 from 1, and no two points closer than 1e-2. A valid certificate
  proves the graph is unit-distance, so it is accepted even if the reference
  answered "No".
- If the reference answered "Yes" (a constructive proof) the submission may
  not answer "No".
- A "No" output is accepted only when the reference also answered "No".

Any embedding satisfying the tolerances is accepted; the reference's own
coordinates are never compared against.
"""

import math
import sys

EPS_EDGE = 1e-7
EPS_NON_EDGE = 1e-2
EPS_DISTINCT = 1e-2
MAX_COORD = 100.0


def reject(message):
    print(message)
    sys.exit(1)


def parse_graph(in_path):
    toks = open(in_path).read().split()
    n = int(toks[0])
    m = int(toks[1])
    adj = [[False] * n for _ in range(n)]
    p = 2
    for _ in range(m):
        u, v = int(toks[p]), int(toks[p + 1])
        p += 2
        adj[u][v] = adj[v][u] = True
    return n, adj


def parse_verdict(tokens):
    if not tokens:
        return None
    return tokens[0].lower()


def validate_certificate(n, adj, tokens):
    # tokens[0] is "yes"; expect 2*n floats afterwards.
    coords = tokens[1:]
    if len(coords) < 2 * n:
        reject(f"expected {n} points (2*n coordinates), got {len(coords)}")
    pts = []
    for i in range(n):
        try:
            x = float(coords[2 * i])
            y = float(coords[2 * i + 1])
        except ValueError:
            reject(f"point {i}: coordinates are not numbers")
        if abs(x) > MAX_COORD or abs(y) > MAX_COORD:
            reject(f"point {i}: coordinate exceeds {MAX_COORD} in absolute value")
        pts.append((x, y))

    for i in range(n):
        for j in range(i + 1, n):
            dx = pts[i][0] - pts[j][0]
            dy = pts[i][1] - pts[j][1]
            d = math.hypot(dx, dy)
            if d < EPS_DISTINCT:
                reject(f"points {i} and {j} are closer than {EPS_DISTINCT}")
            if adj[i][j]:
                if abs(d - 1.0) > EPS_EDGE:
                    reject(f"edge ({i},{j}) has distance {d}, not within "
                           f"{EPS_EDGE} of 1")
            else:
                if abs(d - 1.0) < EPS_NON_EDGE:
                    reject(f"non-edge ({i},{j}) has distance {d}, within "
                           f"{EPS_NON_EDGE} of 1")


def main():
    n, adj = parse_graph(sys.argv[1])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    got = parse_verdict(out_tokens)
    expected = parse_verdict(ans_tokens)

    if got not in ("yes", "no"):
        reject(f"first token must be Yes/No, got '{out_tokens[:1]}'")

    if got == "yes":
        validate_certificate(n, adj, out_tokens)
        sys.exit(0)

    # got == "no"
    if expected == "yes":
        reject("answered No but the graph is unit-distance "
               "(reference produced a valid embedding)")
    sys.exit(0)


if __name__ == "__main__":
    main()
