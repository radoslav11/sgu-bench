"""Checker for p298: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic: when the reference printed -1 the output
must be -1. Otherwise the output must be N integers in [-10000, 10000]
satisfying every promise A[X] >= A[Y] + C, and its A[N] - A[1] must
equal the reference's (the minimum). Any assignment achieving that is
accepted.
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
    raw = [int(t) for t in in_tokens[2:]]
    promises = [(raw[3 * i], raw[3 * i + 1], raw[3 * i + 2]) for i in range(m)]

    if ans_tokens[0] == "-1":
        if out_tokens != ["-1"]:
            reject("no solution exists, expected exactly '-1'")
        sys.exit(0)

    if out_tokens == ["-1"]:
        reject("a solution exists but got -1")
    if len(out_tokens) != n:
        reject(f"expected {n} numbers, got {len(out_tokens)}")
    try:
        a = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer dowry value")

    for v in a:
        if not -10000 <= v <= 10000:
            reject(f"dowry {v} out of [-10000, 10000]")

    for x, y, c in promises:
        if a[x - 1] < a[y - 1] + c:
            reject(
                f"promise violated: A[{x}]={a[x - 1]} < A[{y}]={a[y - 1]} + {c}"
            )

    ref = [int(t) for t in ans_tokens]
    ref_diff = ref[n - 1] - ref[0]
    diff = a[n - 1] - a[0]
    if diff > ref_diff:
        reject(f"A[N]-A[1] = {diff} is not minimal (reference: {ref_diff})")
    if diff < ref_diff:
        reject(
            f"output diff {diff} beats reference {ref_diff}: package bug?"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
