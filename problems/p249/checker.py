"""Checker for p249: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any matrix is accepted that is a permutation of 0..2^(N+M)-1 arranged in
2^N rows by 2^M columns where every pair of side-adjacent cells (the grid
is cyclic in both directions) differs in exactly one binary bit. The
reference answer is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n, m = map(int, open(sys.argv[1]).read().split())
    rows, cols = 1 << n, 1 << m
    total = rows * cols

    try:
        vals = [int(t) for t in open(sys.argv[2]).read().split()]
    except ValueError:
        reject("non-integer token in output")

    if len(vals) != total:
        reject(f"expected {total} numbers, got {len(vals)}")
    if min(vals) < 0 or max(vals) >= total:
        reject("value out of range 0..2^(N+M)-1")
    if len(set(vals)) != total:
        reject("values are not a permutation")

    for r in range(rows):
        base = r * cols
        down = ((r + 1) % rows) * cols
        for c in range(cols):
            v = vals[base + c]
            right = vals[base + (c + 1) % cols]
            below = vals[down + c]
            x = v ^ right
            if x == 0 or x & (x - 1):
                reject(
                    f"cells ({r},{c}) and ({r},{(c + 1) % cols}) "
                    f"differ in more than one bit"
                )
            y = v ^ below
            if y == 0 or y & (y - 1):
                reject(
                    f"cells ({r},{c}) and ({(r + 1) % rows},{c}) "
                    f"differ in more than one bit"
                )

    sys.exit(0)


if __name__ == "__main__":
    main()
