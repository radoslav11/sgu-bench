"""Checker for p541: usage checker.py <in> <out> <ans>; exit 0 = accept.

A valid plan keeps a subset of stations 2-colored (company 1 / company 2)
so that no road joins two stations of the same company. The maximum total
number of kept stations is deterministic, so n1 + n2 must equal the
reference total. The two companies' lists must be disjoint, in range, and
form a proper 2-coloring of the kept induced subgraph.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def read_ints(path):
    return list(map(int, open(path).read().split()))


def main():
    data = read_ints(sys.argv[1])
    n, m = data[0], data[1]
    edges = []
    idx = 2
    for _ in range(m):
        a, b = data[idx], data[idx + 1]
        idx += 2
        edges.append((a, b))

    out = read_ints(sys.argv[2])
    ans = read_ints(sys.argv[3])

    def parse_two_groups(tokens, label):
        pos = 0
        if pos >= len(tokens):
            reject(f"{label}: missing first group count")
        n1 = tokens[pos]
        pos += 1
        if n1 < 0 or pos + n1 > len(tokens):
            reject(f"{label}: bad first group count {n1}")
        g1 = tokens[pos:pos + n1]
        pos += n1
        if pos >= len(tokens):
            reject(f"{label}: missing second group count")
        n2 = tokens[pos]
        pos += 1
        if n2 < 0 or pos + n2 > len(tokens):
            reject(f"{label}: bad second group count {n2}")
        g2 = tokens[pos:pos + n2]
        pos += n2
        if pos != len(tokens):
            reject(f"{label}: trailing tokens after the two groups")
        return g1, g2

    g1, g2 = parse_two_groups(out, "output")
    ref1, ref2 = parse_two_groups(ans, "reference")
    ref_total = len(ref1) + len(ref2)

    color = [0] * (n + 1)  # 0 = removed, 1 = company1, 2 = company2
    for v in g1:
        if not (1 <= v <= n):
            reject(f"company 1 station {v} out of range")
        if color[v] != 0:
            reject(f"station {v} listed more than once")
        color[v] = 1
    for v in g2:
        if not (1 <= v <= n):
            reject(f"company 2 station {v} out of range")
        if color[v] != 0:
            reject(f"station {v} listed more than once")
        color[v] = 2

    for a, b in edges:
        if color[a] != 0 and color[a] == color[b]:
            reject(
                f"road ({a}, {b}) joins two stations of the same company"
            )

    total = len(g1) + len(g2)
    if total != ref_total:
        reject(
            f"total kept stations is {total}, optimum is {ref_total}"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
