"""Checker for p504: usage checker.py <in> <out> <ans>; exit 0 = accept.

The largest palindrome square size is unique, but its location is not, so
"output any" applies. The reference answer gives the maximum size; any
output square is accepted iff it is in-bounds, square, every row and every
column of it is a palindrome, and its side length equals the reference's.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_square(tokens, who):
    if len(tokens) < 4:
        reject(f"{who}: expected 4 integers, got {len(tokens)}")
    try:
        r1, c1, r2, c2 = (int(tokens[i]) for i in range(4))
    except ValueError:
        reject(f"{who}: non-integer coordinate")
    return r1, c1, r2, c2


def is_palindrome(s):
    return s == s[::-1]


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    h, w = int(in_tokens[0]), int(in_tokens[1])
    grid = in_tokens[2:2 + h]
    if len(grid) != h:
        reject("input grid malformed")

    a_r1, a_c1, a_r2, a_c2 = parse_square(ans_tokens, "reference")
    ref_size = a_r2 - a_r1 + 1

    r1, c1, r2, c2 = parse_square(out_tokens, "output")

    if not (1 <= r1 <= r2 <= h):
        reject(f"row range {r1}..{r2} out of bounds (h={h})")
    if not (1 <= c1 <= c2 <= w):
        reject(f"col range {c1}..{c2} out of bounds (w={w})")

    side = r2 - r1 + 1
    if (c2 - c1 + 1) != side:
        reject(
            f"not a square: rows span {side}, cols span {c2 - c1 + 1}"
        )

    if side != ref_size:
        reject(
            f"size {side} is not maximal; reference found size {ref_size}"
        )

    # Validate the square: every row and every column must be a palindrome.
    rows = []
    for i in range(r1 - 1, r2):
        rows.append(grid[i][c1 - 1:c2])
    for idx, row in enumerate(rows):
        if not is_palindrome(row):
            reject(f"row {r1 + idx} of the square is not a palindrome")
    for j in range(side):
        col = "".join(rows[i][j] for i in range(side))
        if not is_palindrome(col):
            reject(f"column {c1 + j} of the square is not a palindrome")

    sys.exit(0)


if __name__ == "__main__":
    main()
