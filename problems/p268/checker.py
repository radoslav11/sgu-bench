"""Checker for p268: usage checker.py <in> <out> <ans>; exit 0 = accept.

The shortest length is deterministic, so the contestant string must be
exactly as long as the reference answer. The string itself may differ:
it is validated directly — it must contain S1 and S2 as substrings and
every window of length N+1 must be almost permutative (the window minus
one character is a permutation of the first N uppercase letters).
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
    s1, s2 = in_tokens[1], in_tokens[2]
    ref = ans_tokens[0]

    if not out_tokens:
        reject("empty output")
    if len(out_tokens) > 1:
        reject("output must be a single string")
    s = out_tokens[0]

    if len(s) != len(ref):
        reject(f"answer length must be {len(ref)}, got {len(s)}")
    if not all("A" <= c <= "Z" for c in s):
        reject("string must consist of capital English letters")
    if s1 not in s:
        reject("S1 is not a substring")
    if s2 not in s:
        reject("S2 is not a substring")

    first_n = set(chr(ord("A") + i) for i in range(n))
    for i in range(len(s) - n):
        window = s[i:i + n + 1]
        # almost permutative <=> each of the first N letters appears,
        # leaving exactly one surplus character to delete
        if not first_n <= set(window):
            reject(f"window at position {i} is not almost permutative")

    sys.exit(0)


if __name__ == "__main__":
    main()
