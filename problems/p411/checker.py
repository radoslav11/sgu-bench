"""Checker for p411: usage checker.py <in> <out> <ans>; exit 0 = accept.

The password length is deterministic and must match the reference
answer's length. The string itself is a certificate: it must be a
palindrome and a (contiguous) substring of both input strings. Any such
string of maximal length is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    a, b = in_tokens[0], in_tokens[1]
    ref = ans_tokens[0]

    if len(out_tokens) != 1:
        reject(f"expected exactly one word, got {len(out_tokens)}")
    out = out_tokens[0]

    if len(out) != len(ref):
        reject(f"optimal length is {len(ref)}, got length {len(out)}")
    if out != out[::-1]:
        reject("output is not a palindrome")
    if out not in a:
        reject("output is not a substring of the first string")
    if out not in b:
        reject("output is not a substring of the second string")

    sys.exit(0)


if __name__ == "__main__":
    main()
