"""Checker for p368: usage checker.py <in> <out> <ans>; exit 0 = accept.

The set of accepted "faculty specialty" lines is determined by the input,
but they may be printed in any order, so we compare the multiset of
non-empty output lines (trailing whitespace stripped) against the
reference answer's multiset.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def lines_of(path):
    return sorted(
        line.rstrip() for line in open(path).read().splitlines()
        if line.strip()
    )


def main():
    out_lines = lines_of(sys.argv[2])
    ans_lines = lines_of(sys.argv[3])

    if out_lines == ans_lines:
        sys.exit(0)

    if len(out_lines) != len(ans_lines):
        reject(f"expected {len(ans_lines)} lines, got {len(out_lines)}")
    for got, exp in zip(out_lines, ans_lines):
        if got != exp:
            reject(f"line mismatch: expected '{exp}', got '{got}'")
    sys.exit(1)


if __name__ == "__main__":
    main()
