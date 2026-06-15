"""Checker for p189: usage checker.py <in> <out> <ans>; exit 0 = accept.

The program output is fully deterministic, but printed values may
contain runs of spaces that a token-wise diff would not notice, so
lines are compared exactly. Trailing '\r' on each line and trailing
blank lines / missing final newline at EOF are forgiven.
"""

import sys


def lines_of(path):
    text = open(path, errors="replace").read()
    lines = [line.rstrip("\r") for line in text.split("\n")]
    while lines and lines[-1] == "":
        lines.pop()
    return lines


def main():
    out_lines = lines_of(sys.argv[2])
    ans_lines = lines_of(sys.argv[3])

    if len(out_lines) != len(ans_lines):
        print(f"expected {len(ans_lines)} lines, got {len(out_lines)}")
        sys.exit(1)
    for i, (got, want) in enumerate(zip(out_lines, ans_lines)):
        if got != want:
            print(f"line {i + 1} differs: expected '{want}', got '{got}'")
            sys.exit(1)
    sys.exit(0)


if __name__ == "__main__":
    main()
