"""Checker for p104: usage checker.py <in> <out> <ans>; exit 0 = accept.

The maximal aesthetic sum is deterministic and must match the reference.
The arrangement is a certificate: F vase indices, strictly increasing,
each in [1, V], whose aesthetic values add up to exactly the claimed
optimal sum. Any such arrangement is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    f, v = int(in_tokens[0]), int(in_tokens[1])
    vals = [int(x) for x in in_tokens[2:]]
    a = [vals[i * v:(i + 1) * v] for i in range(f)]

    best = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        claimed = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the sum, got '{out_tokens[0]}'")
    if claimed != best:
        reject(f"optimal sum is {best}, claimed {claimed}")

    if len(out_tokens) != 1 + f:
        reject(f"expected {f} vase indices, got {len(out_tokens) - 1}")
    try:
        vases = [int(x) for x in out_tokens[1:]]
    except ValueError:
        reject("non-integer vase index")
    if any(not (1 <= x <= v) for x in vases):
        reject("vase index out of range")
    if any(x >= y for x, y in zip(vases, vases[1:])):
        reject("vase indices must be strictly increasing")

    total = sum(a[i][vases[i] - 1] for i in range(f))
    if total != best:
        reject(f"arrangement scores {total}, claimed optimum is {best}")

    sys.exit(0)


if __name__ == "__main__":
    main()
