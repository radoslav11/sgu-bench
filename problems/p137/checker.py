"""Checker for p137: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any funny string counts: N non-negative integers summing to K such that
(S1+1, S2, ..., SN-1, SN-1) is a rotation of (S1, ..., SN). The
reference's string is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n, k = int(in_tokens[0]), int(in_tokens[1])

    if len(out_tokens) != n:
        reject(f"expected {n} integers, got {len(out_tokens)}")
    try:
        s = [int(t) for t in out_tokens]
    except ValueError:
        reject("non-integer element")
    if any(x < 0 for x in s):
        reject("negative element")
    if sum(s) != k:
        reject(f"sum is {sum(s)}, expected {k}")

    t = [s[0] + 1] + s[1:-1] + [s[-1] - 1]
    doubled = s + s
    if not any(doubled[i:i + n] == t for i in range(n)):
        reject("modified string is not a rotation of the original")

    sys.exit(0)


if __name__ == "__main__":
    main()
