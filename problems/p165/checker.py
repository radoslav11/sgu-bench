"""Checker for p165: usage checker.py <in> <out> <ans>; exit 0 = accept.

The yes/no verdict must match the reference. A "yes" must come with a
permutation of 1..N such that every contiguous segment's height sum stays
within 0.10 m of 2.00*K. With prefix sums of the micrometer deviations
from 2.0 m, that is max(prefix) - min(prefix) <= 100000. Any valid
permutation is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def micrometers(token):
    if "." in token:
        left, right = token.split(".", 1)
    else:
        left, right = token, ""
    right = (right + "000000")[:6]
    return int(left) * 1000000 + int(right)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    devs = [micrometers(t) - 2000000 for t in in_tokens[1:1 + n]]

    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].lower()
    expected = ans_tokens[0].lower()
    if got not in ("yes", "no"):
        reject(f"first token must be yes/no, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "no":
        if len(out_tokens) > 1:
            reject("trailing output after 'no'")
        sys.exit(0)

    perm = out_tokens[1:]
    if len(perm) != n:
        reject(f"expected {n} indices, got {len(perm)}")
    try:
        perm = [int(x) for x in perm]
    except ValueError:
        reject("non-integer index in permutation")
    if sorted(perm) != list(range(1, n + 1)):
        reject("not a permutation of 1..N")

    prefix = 0
    lo = hi = 0
    for idx in perm:
        prefix += devs[idx - 1]
        lo = min(lo, prefix)
        hi = max(hi, prefix)
    if hi - lo > 100000:
        reject(f"a segment deviates by {hi - lo} micrometers > 100000")

    sys.exit(0)


if __name__ == "__main__":
    main()
