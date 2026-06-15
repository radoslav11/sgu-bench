"""Checker for p480: usage checker.py <in> <out> <ans>; exit 0 = accept.

A multiset of corners tiles the big corner of size 2^k iff its total
volume (in units of size-1 corners) is exactly 8^k: corner volumes are
powers of 8, so divisible-size bin packing always succeeds. The output
must list each (color, size) type at most once with positive counts not
exceeding availability, sum to exactly 8^k, and use the same number of
distinct colors as the reference (the minimum). "NO SOLUTION" must match
the reference.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    c, k = int(in_tokens[0]), int(in_tokens[1])
    avail = {}
    pos = 2
    for i in range(1, c + 1):
        for j in range(k):
            avail[(i, j)] = int(in_tokens[pos])
            pos += 1

    no_solution = ans_tokens[0].upper() == "NO"

    if not out_tokens:
        reject("empty output")
    if out_tokens[0].upper() == "NO":
        if " ".join(out_tokens).upper() != "NO SOLUTION":
            reject("malformed output starting with 'NO'")
        if not no_solution:
            reject("said 'NO SOLUTION' but a solution exists")
        sys.exit(0)
    if no_solution:
        reject("there is no solution, but output gives a construction")

    try:
        n = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be a count, got '{out_tokens[0]}'")
    if n < 0 or len(out_tokens) != 1 + 3 * n:
        reject(f"expected {1 + 3 * n} tokens for n={n}, got {len(out_tokens)}")

    target = 8**k
    total = 0
    used_colors = set()
    seen_types = set()
    for t in range(n):
        try:
            col = int(out_tokens[1 + 3 * t])
            size = int(out_tokens[2 + 3 * t])
            cnt = int(out_tokens[3 + 3 * t])
        except ValueError:
            reject(f"non-integer tokens in line {t + 2}")
        if not (1 <= col <= c):
            reject(f"color {col} out of range")
        if not (0 <= size < k):
            reject(f"size index {size} out of range")
        if (col, size) in seen_types:
            reject(f"type (color {col}, size {size}) appears twice")
        seen_types.add((col, size))
        if cnt <= 0:
            reject(f"non-positive count for color {col}, size {size}")
        if cnt > avail[(col, size)]:
            reject(
                f"uses {cnt} of color {col}, size {size}; "
                f"only {avail[(col, size)]} available"
            )
        total += cnt * 8**size
        used_colors.add(col)

    if total != target:
        side = "more" if total > target else "less"
        reject(f"total volume is {side} than 8^k, must be exactly 8^k")

    ref_colors = set()
    rn = int(ans_tokens[0])
    for t in range(rn):
        ref_colors.add(int(ans_tokens[1 + 3 * t]))
    if len(used_colors) != len(ref_colors):
        reject(
            f"uses {len(used_colors)} colors, minimum is {len(ref_colors)}"
        )

    sys.exit(0)


if __name__ == "__main__":
    main()
