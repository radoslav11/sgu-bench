"""Checker for p156: usage checker.py <in> <out> <ans>; exit 0 = accept.

Whether a Hamiltonian cycle exists is deterministic, so "-1" must match
the reference. A claimed cycle is validated against the input graph:
it must be a permutation of all N vertices with every consecutive pair
(and the closing pair) joined by an edge. The reference's own cycle is
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
    edges = set()
    for i in range(m):
        u = int(in_tokens[2 + 2 * i])
        v = int(in_tokens[3 + 2 * i])
        edges.add((u, v))
        edges.add((v, u))

    no_cycle = ans_tokens[0] == "-1"
    if not out_tokens:
        reject("empty output")
    if out_tokens[0] == "-1":
        if not no_cycle:
            reject("printed -1 but a hamiltonian cycle exists")
        if len(out_tokens) > 1:
            reject("trailing output after -1")
        sys.exit(0)
    if no_cycle:
        reject("printed a cycle but the answer is -1")

    if len(out_tokens) != n:
        reject(f"expected {n} vertices, got {len(out_tokens)} tokens")
    try:
        cyc = [int(x) for x in out_tokens]
    except ValueError:
        reject("non-integer vertex in output")
    if sorted(cyc) != list(range(1, n + 1)):
        reject("output is not a permutation of 1..N")
    for i in range(n):
        u, v = cyc[i], cyc[(i + 1) % n]
        if (u, v) not in edges:
            reject(f"consecutive vertices {u} {v} are not adjacent")

    sys.exit(0)


if __name__ == "__main__":
    main()
