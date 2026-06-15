"""Checker for p502: usage checker.py <in> <out> <ans>; exit 0 = accept.

Existence of a valid permutation is deterministic, so the reference's
verdict ("-1" vs a number) must match: if the reference outputs -1 the
solution must too, and vice versa. When a permutation exists, any output
that (1) uses exactly the multiset of input digits, (2) has no leading
zero, and (3) is divisible by 17 is accepted; the certificate is never
required to equal the reference's.
"""

import sys


def reject(msg):
    print(msg)
    sys.exit(1)


def main():
    inp = open(sys.argv[1]).read().split()
    out = open(sys.argv[2]).read().split()
    ans = open(sys.argv[3]).read().split()

    if not inp:
        reject("empty input")
    n = inp[0]

    if not ans:
        reject("empty reference answer")
    ref = ans[0]

    if not out:
        reject("empty output")
    if len(out) > 1:
        reject("expected a single token")
    got = out[0]

    ref_is_neg = ref == "-1"
    got_is_neg = got == "-1"

    if ref_is_neg != got_is_neg:
        if ref_is_neg:
            reject("reference says no permutation exists, output gave one")
        else:
            reject("a valid permutation exists, output gave -1")

    if got_is_neg:
        sys.exit(0)

    # validate the produced number
    if not got.isdigit():
        reject(f"output '{got}' is not a non-negative integer")

    # leading zero allowed only for the single-digit number 0
    if len(got) > 1 and got[0] == "0":
        reject("output starts with a zero")

    if sorted(got) != sorted(n):
        reject("output is not a permutation of the input digits")

    if int(got) % 17 != 0:
        reject("output is not divisible by 17")

    sys.exit(0)


if __name__ == "__main__":
    main()
