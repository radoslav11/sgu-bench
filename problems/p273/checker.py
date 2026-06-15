"""Checker for p273: usage checker.py <in> <out> <ans>; exit 0 = accept.

The set of winnable colors is deterministic, but the statement allows the
colors to be printed in any order, so the output is compared to the
reference answer as a SET of color characters. "Nobody" must match
exactly (case-insensitively) when the reference set is empty.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    expected = ans_tokens[0]
    if not out_tokens:
        reject("empty output")
    if len(out_tokens) > 1:
        reject("more than one token in output")
    got = out_tokens[0]

    if expected.lower() == "nobody":
        if got.lower() != "nobody":
            reject(f"expected 'Nobody', got '{got}'")
        sys.exit(0)

    if got.lower() == "nobody":
        reject(f"expected colors '{expected}', got 'Nobody'")
    if len(set(got)) != len(got):
        reject(f"duplicate colors in '{got}'")
    if any(c not in "bryw" for c in got):
        reject(f"invalid color character in '{got}'")
    if set(got) != set(expected):
        reject(f"expected color set '{expected}', got '{got}'")

    sys.exit(0)


if __name__ == "__main__":
    main()
