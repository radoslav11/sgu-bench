"""Checker for p286: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO verdict must match the reference. A YES must come with N road
indices (a 2-factor): distinct valid indices such that every city is the
endpoint of exactly two chosen roads. Any valid 2-factor is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, k = int(in_tokens[0]), int(in_tokens[1])
    m = n * k // 2
    edges = []
    pos = 2
    for _ in range(m):
        edges.append((int(in_tokens[pos]), int(in_tokens[pos + 1])))
        pos += 2

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    rest = out_tokens[1:]
    if len(rest) != n:
        reject(f"expected {n} road numbers, got {len(rest)}")
    try:
        chosen = [int(x) for x in rest]
    except ValueError:
        reject("non-integer road number")
    if len(set(chosen)) != n:
        reject("duplicate road numbers")
    if any(not (1 <= e <= m) for e in chosen):
        reject("road number out of range")

    deg = [0] * (n + 1)
    for e in chosen:
        u, v = edges[e - 1]
        deg[u] += 1
        deg[v] += 1
    for v in range(1, n + 1):
        if deg[v] != 2:
            reject(f"city {v} has {deg[v]} decorated roads, expected 2")

    sys.exit(0)


if __name__ == "__main__":
    main()
