"""Checker for p327: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any shortest palindrome works: the output must be one lowercase word,
read the same backwards, contain every input word as a substring, and
have the same length as the reference answer (which is optimal).
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
    words = in_tokens[1:1 + n]

    if len(out_tokens) != 1:
        reject(f"expected exactly one word, got {len(out_tokens)} tokens")
    pal = out_tokens[0]

    if not pal.islower() or not pal.isalpha():
        reject("output contains characters other than lowercase letters")
    if pal != pal[::-1]:
        reject("output is not a palindrome")
    for w in words:
        if w not in pal:
            reject(f"word '{w}' is not a substring of the output")
    if len(pal) != len(ans_tokens[0]):
        reject(
            f"length {len(pal)} differs from optimal {len(ans_tokens[0])}"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
