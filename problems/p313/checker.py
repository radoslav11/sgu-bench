"""Checker for p313: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal total travel time must match the reference's first number.
The assignment is any permutation of office indices whose total circular
distance equals that minimum; the reference's own permutation is never
compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_total = int(open(sys.argv[3]).read().split()[0])

    n, length = int(in_tokens[0]), int(in_tokens[1])
    houses = [int(x) for x in in_tokens[2:2 + n]]
    offices = [int(x) for x in in_tokens[2 + n:2 + 2 * n]]

    if len(out_tokens) != n + 1:
        reject(f"expected {n + 1} numbers, got {len(out_tokens)}")
    try:
        total = int(out_tokens[0])
        perm = [int(x) for x in out_tokens[1:]]
    except ValueError:
        reject("non-integer token in output")

    if total != ans_total:
        reject(f"minimal total is {ans_total}, claimed {total}")
    if sorted(perm) != list(range(1, n + 1)):
        reject("assignment is not a permutation of 1..n")

    real = 0
    for h, idx in zip(houses, perm):
        d = abs(h - offices[idx - 1])
        real += min(d, length - d)
    if real != total:
        reject(f"assignment costs {real}, claimed {total}")
    sys.exit(0)


if __name__ == "__main__":
    main()
