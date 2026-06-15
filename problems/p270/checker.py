"""Checker for p270: usage checker.py <in> <out> <ans>; exit 0 = accept.

The set of reachable thimbles is deterministic but may be printed in
any order, so the output is compared to the reference answer as a set
of integers; duplicates are rejected.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    try:
        got = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer token in output")

    if len(set(got)) != len(got):
        reject("duplicate numbers in output")

    expected = set(int(t) for t in ans_tokens)
    if set(got) != expected:
        missing = sorted(expected - set(got))
        extra = sorted(set(got) - expected)
        reject(f"wrong set: missing {missing[:5]}, extra {extra[:5]}")

    sys.exit(0)


if __name__ == "__main__":
    main()
