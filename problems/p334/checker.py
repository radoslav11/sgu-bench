"""Checker for p334: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal number of parts K is deterministic and must match the
reference. The rearrangement is a certificate: the H x W picture must
relabel exactly the 'X' cells with the first K letters, every part must
be 4-connected, and the 3 x 3 picture must use translated copies of the
same parts to tile the square. Any valid rearrangement is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def cells_of(lines, letter):
    return sorted(
        (r, c)
        for r, row in enumerate(lines)
        for c, ch in enumerate(row)
        if ch == letter
    )


def connected(cells):
    if not cells:
        return False
    cell_set = set(cells)
    stack = [cells[0]]
    seen = {cells[0]}
    while stack:
        r, c = stack.pop()
        for nr, nc in ((r + 1, c), (r - 1, c), (r, c + 1), (r, c - 1)):
            if (nr, nc) in cell_set and (nr, nc) not in seen:
                seen.add((nr, nc))
                stack.append((nr, nc))
    return len(seen) == len(cells)


def main():
    in_lines = open(sys.argv[1]).read().splitlines()
    out_lines = [l.rstrip() for l in open(sys.argv[2]).read().splitlines()]
    ans_tokens = open(sys.argv[3]).read().split()

    h, w = map(int, in_lines[0].split())
    logo = [in_lines[1 + i].rstrip() for i in range(h)]
    ref_k = int(ans_tokens[0])

    nonempty = [l for l in out_lines if l.strip()]
    if len(nonempty) != 1 + h + 3:
        reject(f"expected {1 + h + 3} nonempty lines, got {len(nonempty)}")

    try:
        k = int(nonempty[0].strip())
    except ValueError:
        reject(f"first line must be K, got '{nonempty[0]}'")
    if k != ref_k:
        reject(f"K={k}, minimal is {ref_k}")

    letters = [chr(ord("A") + i) for i in range(k)]
    pic = nonempty[1 : 1 + h]
    square = nonempty[1 + h : 1 + h + 3]

    for r in range(h):
        if len(pic[r]) != w:
            reject(f"picture line {r + 1} has length {len(pic[r])}, not {w}")
        for c in range(w):
            if logo[r][c] == "X":
                if pic[r][c] not in letters:
                    reject(f"cell ({r},{c}) must be one of first {k} letters")
            elif pic[r][c] != ".":
                reject(f"cell ({r},{c}) must stay '.'")

    for r in range(3):
        if len(square[r]) != 3:
            reject(f"square line {r + 1} is not 3 characters")
        for c in range(3):
            if square[r][c] not in letters:
                reject(f"square cell ({r},{c}) is not a part letter")

    for letter in letters:
        part = cells_of(pic, letter)
        if not part:
            reject(f"part {letter} is missing from the picture")
        if not connected(part):
            reject(f"part {letter} is not connected")
        target = cells_of(square, letter)
        if len(target) != len(part):
            reject(f"part {letter} changed size in the square")
        dr = target[0][0] - part[0][0]
        dc = target[0][1] - part[0][1]
        if any(
            (r + dr, c + dc) != t for (r, c), t in zip(part, target)
        ):
            reject(f"part {letter} is not a pure translation")

    sys.exit(0)


if __name__ == "__main__":
    main()
