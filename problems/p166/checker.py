"""Checker for p166: usage checker.py <in> <out> <ans>; exit 0 = accept.

Spaces inside lines and line boundaries are both significant in the
editor's output, so a token-wise diff is too lenient. Lines must match
the reference exactly; CR/LF variations and one trailing newline at EOF
are forgiven.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def lines_of(path):
    text = open(path, newline="").read()
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    if text.endswith("\n"):
        text = text[:-1]
    return text.split("\n")


def main():
    out_lines = lines_of(sys.argv[2])
    ans_lines = lines_of(sys.argv[3])

    if len(out_lines) != len(ans_lines):
        reject(f"expected {len(ans_lines)} lines, got {len(out_lines)}")
    for i, (got, want) in enumerate(zip(out_lines, ans_lines)):
        if got != want:
            reject(f"line {i + 1} differs: expected {want!r}, got {got!r}")

    sys.exit(0)


if __name__ == "__main__":
    main()
