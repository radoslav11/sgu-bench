"""Checker for p194: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO part is deterministic and must match the reference. A YES
must be followed by M integers f_k with l_k <= f_k <= c_k and, at every
node, equal inflow and outflow. Any valid circulation is accepted; the
reference's own flow values are never compared against.
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
    for k in range(m):
        u, v, lo, hi = (int(x) for x in in_tokens[2 + 4 * k:6 + 4 * k])
        edges.append((u, v, lo, hi))

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

    if len(out_tokens) != 1 + m:
        reject(f"expected {m} flow values, got {len(out_tokens) - 1}")
    try:
        flows = [int(x) for x in out_tokens[1:]]
    except ValueError:
        reject("non-integer flow value")

    balance = [0] * (n + 1)
    for (u, v, lo, hi), f in zip(edges, flows):
        if not lo <= f <= hi:
            reject(f"flow {f} on pipe {u}->{v} outside [{lo}, {hi}]")
        balance[u] -= f
        balance[v] += f
    for i in range(1, n + 1):
        if balance[i] != 0:
            reject(f"node {i} has net flow {balance[i]}")

    sys.exit(0)


if __name__ == "__main__":
    main()
