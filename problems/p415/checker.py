"""Checker for p415: usage checker.py <in> <out> <ans>; exit 0 = accept.

The set of necessary coins is unique, but the statement allows printing
the denominations in any order, so the output is compared to the
reference answer as a set (count first, then the sorted values).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    try:
        expected = sorted(int(t) for t in ans_tokens[1:])
    except ValueError:
        reject("internal: malformed reference answer")

    if not out_tokens:
        reject("empty output")
    try:
        k = int(out_tokens[0])
        got = sorted(int(t) for t in out_tokens[1:])
    except ValueError:
        reject("non-integer token in output")

    if k != len(expected):
        reject(f"expected count {len(expected)}, got {k}")
    if len(got) != k:
        reject(f"declared {k} coins, listed {len(got)}")
    if got != expected:
        reject("coin set differs from the necessary set")

    sys.exit(0)


if __name__ == "__main__":
    main()
