"""Checker for p299: usage checker.py <in> <out> <ans>; exit 0 = accept.

Whether a triangle exists is deterministic ("0 0 0" must match), but any
three input segments that form a non-degenerate triangle are accepted:
the three printed lengths must be a sub-multiset of the input lengths
and satisfy the strict triangle inequality.
"""

import sys
from collections import Counter


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    lengths = Counter(int(t) for t in in_tokens[1 : n + 1])

    if len(out_tokens) != 3:
        reject(f"expected exactly 3 numbers, got {len(out_tokens)}")
    try:
        triple = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer output token")

    expected_none = [int(t) for t in ans_tokens] == [0, 0, 0]
    if expected_none:
        if triple != [0, 0, 0]:
            reject("no triangle exists, expected '0 0 0'")
        sys.exit(0)

    if triple == [0, 0, 0]:
        reject("a triangle exists but got '0 0 0'")

    need = Counter(triple)
    for v, cnt in need.items():
        if lengths[v] < cnt:
            reject(f"length {v} used more times than present in input")

    a, b, c = sorted(triple)
    if a + b <= c:
        reject(f"not a triangle: {a} + {b} <= {c}")

    sys.exit(0)


if __name__ == "__main__":
    main()
