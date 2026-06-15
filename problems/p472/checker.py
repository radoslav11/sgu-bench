"""Checker for p472: usage checker.py <in> <out> <ans>; exit 0 = accept.

Any valid move sequence is accepted as long as it solves the maze with
the same number of pushes and the same total number of moves as the
reference answer (which is optimal lexicographically by (pushes,
moves)). "Impossible." is deterministic and must match the reference.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    raw = open(sys.argv[1]).read().split("\n")
    while raw and raw[-1] == "":
        raw.pop()
    rows = len(raw)
    cols = max(len(line) for line in raw)
    grid = [list(line.ljust(cols, "#")) for line in raw]

    keeper = box = target = None
    for r in range(rows):
        for c in range(cols):
            ch = grid[r][c]
            if ch == "@":
                keeper = (r, c)
                grid[r][c] = " "
            elif ch == "$":
                box = (r, c)
                grid[r][c] = " "
            elif ch == ".":
                target = (r, c)
                grid[r][c] = " "

    ans = open(sys.argv[3]).read().strip()
    out = open(sys.argv[2]).read().strip()

    if ans == "Impossible.":
        if out != "Impossible.":
            reject("maze is unsolvable, expected 'Impossible.'")
        sys.exit(0)

    if out == "Impossible.":
        reject("maze is solvable, got 'Impossible.'")

    moves = out
    deltas = {"u": (-1, 0), "d": (1, 0), "l": (0, -1), "r": (0, 1)}
    pushes = 0
    for idx, ch in enumerate(moves):
        if ch.lower() not in deltas:
            reject(f"invalid move character '{ch}' at position {idx}")
        dr, dc = deltas[ch.lower()]
        nr, nc = keeper[0] + dr, keeper[1] + dc
        if not (0 <= nr < rows and 0 <= nc < cols) or grid[nr][nc] == "#":
            reject(f"move {idx} ('{ch}') walks into a wall")
        if ch.isupper():
            if (nr, nc) != box:
                reject(f"move {idx} ('{ch}') is a push but no box is there")
            br, bc = nr + dr, nc + dc
            if not (0 <= br < rows and 0 <= bc < cols) or grid[br][bc] == "#":
                reject(f"move {idx} ('{ch}') pushes the box into a wall")
            box = (br, bc)
            pushes += 1
        else:
            if (nr, nc) == box:
                reject(f"move {idx} ('{ch}') bumps into the box without push")
        keeper = (nr, nc)

    if box != target:
        reject("box does not end on the destination cell")

    ans_pushes = sum(1 for ch in ans if ch.isupper())
    if pushes != ans_pushes:
        reject(f"expected {ans_pushes} pushes, got {pushes}")
    if len(moves) != len(ans):
        reject(f"expected {len(ans)} moves, got {len(moves)}")

    sys.exit(0)


if __name__ == "__main__":
    main()
