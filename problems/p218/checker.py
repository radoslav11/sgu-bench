"""Checker for p218: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal possible maximum unsafety is unique and must match the
reference's first line. The assignment itself may be any perfect
matching between computers and programs whose maximum unsafety equals
that optimum; it is validated against the input matrix, never against
the reference's own assignment.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_raw = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    matrix = [
        [int(in_tokens[1 + i * n + j]) for j in range(n)] for i in range(n)
    ]
    optimum = int(ans_tokens[0])

    try:
        out_tokens = [int(t) for t in out_raw]
    except ValueError:
        reject("output contains non-integer tokens")
    if len(out_tokens) != 1 + 2 * n:
        reject(f"expected {1 + 2 * n} tokens, got {len(out_tokens)}")

    value = out_tokens[0]
    if value != optimum:
        reject(f"optimal maximal unsafety is {optimum}, got {value}")

    seen_i, seen_j = set(), set()
    worst = None
    for k in range(n):
        i, j = out_tokens[1 + 2 * k], out_tokens[2 + 2 * k]
        if not (1 <= i <= n) or not (1 <= j <= n):
            reject(f"pair ({i}, {j}) out of range 1..{n}")
        if i in seen_i:
            reject(f"computer {i} assigned twice")
        if j in seen_j:
            reject(f"program {j} assigned twice")
        seen_i.add(i)
        seen_j.add(j)
        w = matrix[i - 1][j - 1]
        if worst is None or w > worst:
            worst = w

    if worst > value:
        reject(f"assignment has maximal unsafety {worst}, claimed {value}")

    sys.exit(0)


if __name__ == "__main__":
    main()
