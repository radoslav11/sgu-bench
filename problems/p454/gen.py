"""Tests for p454 (Kakuro): n, m <= 6, top row / left column all black.

A solved grid is generated first (white pattern + digits with distinct
digits per run), then clues are derived from the digit sums, so every
test is solvable by construction. Seed 1 is the minimal 2x2 puzzle,
seeds 2-3 are 6x6 with a fully white interior (the largest search
space), later seeds mix random sizes and black-cell densities.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    d = rng.randint(1, 9)
    print(2, 2)
    print("XXXXX", f"0{d}\\XX")
    print(f"XX\\0{d}", ".....")
    sys.exit()

if seed in (2, 3):
    n, m = 6, 6
    white_prob = 1.1  # fully white interior
elif seed == 4:
    n, m = 6, 6
    white_prob = 0.55
else:
    n, m = rng.randint(2, 6), rng.randint(2, 6)
    white_prob = rng.uniform(0.4, 0.95)

while True:
    white = [[False] * m for _ in range(n)]
    for i in range(1, n):
        for j in range(1, m):
            white[i][j] = rng.random() < white_prob
    if any(any(row) for row in white):
        break

# Fill white cells with digits, distinct within every maximal run.
cells = [(i, j) for i in range(n) for j in range(m) if white[i][j]]
digit = [[0] * m for _ in range(n)]


def fill(k):
    if k == len(cells):
        return True
    i, j = cells[k]
    used = set()
    jj = j - 1
    while jj >= 0 and white[i][jj]:
        used.add(digit[i][jj])
        jj -= 1
    ii = i - 1
    while ii >= 0 and white[ii][j]:
        used.add(digit[ii][j])
        ii -= 1
    cand = [d for d in range(1, 10) if d not in used]
    rng.shuffle(cand)
    for d in cand:
        digit[i][j] = d
        if fill(k + 1):
            return True
    digit[i][j] = 0
    return False


assert fill(0)

print(n, m)
for i in range(n):
    row = []
    for j in range(m):
        if white[i][j]:
            row.append(".....")
            continue
        aa, bb = "XX", "XX"
        if i + 1 < n and white[i + 1][j]:
            s, ii = 0, i + 1
            while ii < n and white[ii][j]:
                s += digit[ii][j]
                ii += 1
            aa = f"{s:02d}"
        if j + 1 < m and white[i][j + 1]:
            s, jj = 0, j + 1
            while jj < m and white[i][jj]:
                s += digit[i][jj]
                jj += 1
            bb = f"{s:02d}"
        row.append("XXXXX" if aa == "XX" and bb == "XX" else f"{aa}\\{bb}")
    print(" ".join(row))
