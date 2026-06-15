"""Checker for p109: usage checker.py <in> <out> <ans>; exit 0 = accept.

The whole output is a certificate. The set of possible finger positions
starts as {picture 1} and is evolved turn by turn as a bitmask over the
N x N grid: K exact moves through still-present pictures, then the
removed pictures must avoid every possible position. Each K_i must be
in [N, 300), all K_i distinct, removed pictures must still exist, and
exactly one picture must remain at the end. The reference output is
never compared against.

One leniency: a turn that removes zero pictures is tolerated (the
trusted reference itself emits such a turn for N = 2).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_lines = [l.split() for l in open(sys.argv[2]).read().splitlines()]
    out_lines = [l for l in out_lines if l]

    cells = n * n
    full = (1 << cells) - 1
    col0 = sum(1 << (i * n) for i in range(n))
    col_last = col0 << (n - 1)

    def shift_union(s):
        return (((s & ~col0) >> 1) | ((s & ~col_last) << 1)
                | (s >> n) | ((s << n) & full))

    if not out_lines:
        reject("empty output")

    remaining = full
    possible = 1  # finger starts on picture 1 (bit 0)
    used_k = set()

    for line_no, tokens in enumerate(out_lines, 1):
        try:
            nums = [int(t) for t in tokens]
        except ValueError:
            reject(f"line {line_no}: non-integer token")
        k, removed = nums[0], nums[1:]

        if not (n <= k < 300):
            reject(f"line {line_no}: K={k} not in [N, 300)")
        if k in used_k:
            reject(f"line {line_no}: duplicate K={k}")
        used_k.add(k)

        avail = shift_union(remaining)
        for _ in range(k):
            if possible & ~avail:
                reject(f"line {line_no}: a possible finger position "
                       "has no adjacent picture to move to")
            possible = shift_union(possible) & remaining

        if len(set(removed)) != len(removed):
            reject(f"line {line_no}: picture listed twice")
        for x in removed:
            if not (1 <= x <= cells):
                reject(f"line {line_no}: picture {x} out of range")
            bit = 1 << (x - 1)
            if not (remaining & bit):
                reject(f"line {line_no}: picture {x} already removed")
            if possible & bit:
                reject(f"line {line_no}: finger may be on removed "
                       f"picture {x}")
            remaining &= ~bit

    if bin(remaining).count("1") != 1:
        reject(f"{bin(remaining).count('1')} pictures remain, expected 1")

    sys.exit(0)


if __name__ == "__main__":
    main()
