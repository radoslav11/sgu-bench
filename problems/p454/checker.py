"""Checker for p454 (Kakuro): usage checker.py <in> <out> <ans>; exit 0 = accept.

Any valid solution is accepted: the output grid must mark black cells
with '_', put a digit 1-9 in every white cell, and every run (maximal
horizontal/vertical white block) must consist of distinct digits whose
sum equals its clue. The reference answer is never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    g = [in_tokens[2 + i * m : 2 + (i + 1) * m] for i in range(n)]

    if len(out_tokens) != n * m:
        reject(f"expected {n * m} cells in output, got {len(out_tokens)}")

    val = [[0] * m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            tok = out_tokens[i * m + j]
            if g[i][j][0] == ".":
                if len(tok) != 1 or tok not in "123456789":
                    reject(f"white cell ({i},{j}) must be a digit 1-9, got '{tok}'")
                val[i][j] = int(tok)
            else:
                if tok != "_":
                    reject(f"black cell ({i},{j}) must be '_', got '{tok}'")

    for i in range(n):
        for j in range(m):
            s = g[i][j]
            if s[0] == "." or s == "XXXXX":
                continue
            aa, bb = s[:2], s[3:5]
            if bb != "XX":
                run = []
                k = j + 1
                while k < m and g[i][k][0] == ".":
                    run.append(val[i][k])
                    k += 1
                if len(set(run)) != len(run):
                    reject(f"repeated digit in horizontal run at ({i},{j})")
                if sum(run) != int(bb):
                    reject(
                        f"horizontal run at ({i},{j}): sum {sum(run)} != clue {int(bb)}"
                    )
            if aa != "XX":
                run = []
                k = i + 1
                while k < n and g[k][j][0] == ".":
                    run.append(val[k][j])
                    k += 1
                if len(set(run)) != len(run):
                    reject(f"repeated digit in vertical run at ({i},{j})")
                if sum(run) != int(aa):
                    reject(
                        f"vertical run at ({i},{j}): sum {sum(run)} != clue {int(aa)}"
                    )

    sys.exit(0)


if __name__ == "__main__":
    main()
