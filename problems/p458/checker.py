"""Checker for p458: usage checker.py <in> <out> <ans>; exit 0 = accept.

The deletion count t is deterministic and must match the reference. The
remaining picture is a certificate: it must contain exactly n - t
numbers, be a subsequence of the original pixels, and have no adjacent
pair differing by exactly 1. Any such sequence is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    c = [int(x) for x in in_tokens[1:1 + n]]

    expected_t = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        t = int(out_tokens[0])
    except ValueError:
        reject(f"deletion count is not an integer: '{out_tokens[0]}'")
    if t != expected_t:
        reject(f"minimum deletions is {expected_t}, got {t}")

    m = n - t
    try:
        r = [int(x) for x in out_tokens[1:]]
    except ValueError:
        reject("picture contains a non-integer token")
    if len(r) != m:
        reject(f"expected {m} pixels, got {len(r)}")

    for i in range(m - 1):
        if abs(r[i] - r[i + 1]) == 1:
            reject(
                f"adjacent pixels {r[i]} and {r[i + 1]} differ by exactly 1"
            )

    pos = 0
    for x in r:
        while pos < n and c[pos] != x:
            pos += 1
        if pos == n:
            reject(f"output is not a subsequence of the picture (at {x})")
        pos += 1

    sys.exit(0)


if __name__ == "__main__":
    main()
