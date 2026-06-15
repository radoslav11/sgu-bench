"""Checker for p229: usage checker.py <in> <out> <ans>; exit 0 = accept.

YES/NO must match the reference. A "YES" must come with an N x N 0/1
grid describing one part S of Q; the checker verifies S is a subset of
Q and that S equals Q \\ S after some rotation by a multiple of 90
degrees plus a translation. Any valid division is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def normalized(cells):
    if not cells:
        return frozenset()
    mr = min(r for r, c in cells)
    mc = min(c for r, c in cells)
    return frozenset((r - mr, c - mc) for r, c in cells)


def main():
    in_lines = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_lines[0])
    grid = in_lines[1 : 1 + n]
    q = {(r, c) for r in range(n) for c in range(n) if grid[r][c] == "1"}

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        sys.exit(0)

    rows = out_tokens[1:]
    if len(rows) != n:
        reject(f"expected {n} grid rows after YES, got {len(rows)}")
    s = set()
    for r, row in enumerate(rows):
        if len(row) != n or any(ch not in "01" for ch in row):
            reject(f"row {r + 1} is not {n} binary characters")
        for c, ch in enumerate(row):
            if ch == "1":
                s.add((r, c))

    if not s <= q:
        reject("subset contains a cell outside Q")
    t = q - s
    if len(s) != len(t):
        reject(f"parts have different sizes: {len(s)} vs {len(t)}")

    target = normalized(s)
    cur = t
    for _ in range(4):
        if normalized(cur) == target:
            sys.exit(0)
        cur = {(c, -r) for r, c in cur}

    reject("complement is not a rotation+shift image of the subset")


if __name__ == "__main__":
    main()
