"""Checker for p433: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO verdict is deterministic and must match the reference. A YES
must come with an N x M grid of lowercase letters; maximal same-letter
connected components (4-adjacency) are the bricks, and each must be a
straight 1 x L segment whose L is one of the given brick lengths. Any
valid tiling is accepted, not just the reference's.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_lines = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, k = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    lengths = set(int(x) for x in in_tokens[3:3 + k])

    if not out_lines:
        reject("empty output")
    expected = ans_tokens[0].upper()
    got = out_lines[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_lines[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_lines) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    rows = out_lines[1:]
    if len(rows) != n:
        reject(f"expected {n} grid rows, got {len(rows)}")
    for row in rows:
        if len(row) != m:
            reject(f"row '{row}' does not have length {m}")
        if not row.islower() or not row.isalpha():
            reject(f"row '{row}' has non-lowercase-letter characters")

    seen = [[False] * m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            if seen[i][j]:
                continue
            # Flood-fill the same-letter component starting at (i, j).
            ch = rows[i][j]
            stack = [(i, j)]
            seen[i][j] = True
            cells = []
            while stack:
                r, c = stack.pop()
                cells.append((r, c))
                for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                    nr, nc = r + dr, c + dc
                    if (0 <= nr < n and 0 <= nc < m and not seen[nr][nc]
                            and rows[nr][nc] == ch):
                        seen[nr][nc] = True
                        stack.append((nr, nc))
            rs = sorted(set(r for r, _ in cells))
            cs = sorted(set(c for _, c in cells))
            length = len(cells)
            straight = (
                (len(rs) == 1 and cs == list(range(cs[0], cs[0] + length)))
                or (len(cs) == 1 and rs == list(range(rs[0], rs[0] + length)))
            )
            if not straight:
                reject(f"component of '{ch}' at ({i},{j}) is not a 1xL strip")
            if length not in lengths:
                reject(
                    f"brick of '{ch}' at ({i},{j}) has length {length}, "
                    f"not an allowed brick length"
                )

    sys.exit(0)


if __name__ == "__main__":
    main()
