"""Checker for p159: usage checker.py <in> <out> <ans>; exit 0 = accept.

The set of self-replicating numbers is unique, only the listing order
is free. So K must match the reference and the multiset of printed
numbers must equal the reference's multiset (compared as exact tokens).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    k = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        got_k = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be K, got '{out_tokens[0]}'")
    if got_k != k:
        reject(f"K is {k}, got {got_k}")
    if len(out_tokens) != 1 + k:
        reject(f"expected {k} numbers after K, got {len(out_tokens) - 1}")
    if sorted(out_tokens[1:]) != sorted(ans_tokens[1:]):
        reject("set of self-replicating numbers differs from the answer")

    sys.exit(0)


if __name__ == "__main__":
    main()
